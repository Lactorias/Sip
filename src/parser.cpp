#include <ErrLog.hpp>
#include <Expr.hpp>
#include <Token.hpp>
#include <Stmt.hpp>
#include <vector>
#include <Parser.hpp>

/*
    Sip's parsing favors and implements left recursion as a recursive descent parser, we use C style operator precedence and associativity rules. 
    The following functions highlight each key piece of our syntactic grammar that we will parse, the precedence is 
    implemented from lowest to highest as seen in our function formatting, going from
    equality to unary in precedence.

    Each function is a component of our grammar, and has the ability to relate a subexpression at its presedence level or higher.
    Recursive descent parsing allows us to effectively do a 1:1 conversion between our grammar to code.

    expression     → equality ;
    equality       → comparison ( ( "!=" | "==" ) comparison )* ;
    comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
    term           → factor ( ( "-" | "+" ) factor )* ;
    factor         → unary ( ( "/" | "*" ) unary )* ;
    unary          → ( "!" | "-" ) unary
                    | primary ;
    primary        → NUMBER | STRING | "true" | "false" | "nil"
                    |"(" expression ")" ;

    The parser operates similarly to the scanner, except instead of reading source code, we are now reading our scanned tokens in the
    parser.
*/

/*
    expression(), The highest level of our recursive descent parsing, this can go into any piece of our grammar,
    as we stated that subexpressions can go to their presendence level or higher, so since we start at the bottom, we can
    go anywhere.
*/
inline auto Parser::expression() -> unique_ptr<Expr> {
    return assignment(); 
}

auto Parser::assignment() -> unique_ptr<Expr> {
    auto expr = _or();
    if (match(Token::TokenType::EQUAL)) {
        auto equals = previous();
        auto value = assignment();
        if (auto var_expr = dynamic_cast<Variable*>(expr.get())) {
            auto name = std::move(*var_expr).name;
            return std::make_unique<Assign>(std::move(name), std::move(value));
        }
        error(*equals, "Invalid assignment target.");
    }
    return expr;
}

auto Parser::_or() -> unique_ptr<Expr> {
    auto expr = _and();
    while (match(Token::TokenType::OR)) {
        auto op = previous();
        auto right = _and();
        expr = std::make_unique<Logical>(std::move(expr), std::move(op), std::move(right));
    }
    return expr;
}

auto Parser::_and() -> unique_ptr<Expr> {
    auto expr = equality();
    while (match(Token::TokenType::AND)) {
        auto op = previous();
        auto right = equality();
        expr = std::make_unique<Logical>(std::move(expr), std::move(op), std::move(right));
    }
    return expr;
}

/*
    parse(), kicks off our parsing, starting at the 'lowest' level "expression".    
*/
auto Parser::parse() -> std::vector<unique_ptr<Stmt>> {
    auto statements = std::vector<unique_ptr<Stmt>>();
    while (!at_end()) statements.push_back(declaration());
    return statements;
}

auto Parser::declaration() -> unique_ptr<Stmt> {
    try {
        if (match(Token::TokenType::VAR)) return var_declaration();
        return statement();
    } catch (ErrLog error) {
        synchronize();
        return nullptr;
    }
}

auto Parser::var_declaration() -> unique_ptr<Stmt> {
    auto name = consume(Token::TokenType::IDENTIFIER, "Expect variable name.");
    unique_ptr<Expr> initializer;
    if (match(Token::TokenType::EQUAL)) initializer = expression();
    consume(Token::TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<Var>(std::move(name), std::move(initializer));
}

auto Parser::statement() -> unique_ptr<Stmt> {
    if (match(Token::TokenType::FOR)) return for_statement();
    if (match(Token::TokenType::IF)) return if_statement();
    if (match(Token::TokenType::PRINT)) return print_statement();
    if (match(Token::TokenType::WHILE)) return while_statement();
    if (match(Token::TokenType::LEFT_BRACE)) return std::make_unique<Block>(block());
    return expression_statement();
}

auto Parser::for_statement() -> unique_ptr<Stmt> {
    consume(Token::TokenType::LEFT_PAREN, "Expect '(' after 'for'.");
    std::unique_ptr<Stmt> initializer;
    if (match(Token::TokenType::SEMICOLON)) {
        initializer = nullptr;
    } else if (match(Token::TokenType::VAR)) {
        initializer = std::move(var_declaration());
    } else {
        initializer = std::move(expression_statement());
    }
    unique_ptr<Expr> condition = nullptr;
    if (!check(Token::TokenType::SEMICOLON)) {
        condition = std::move(expression());
    }
    consume(Token::TokenType::SEMICOLON, "Expect ';' after loop condiiton.");
    unique_ptr<Expr> increment;
    if (!check(Token::TokenType::RIGHT_PAREN)) {
        increment = std::move(expression());
    }
    consume(Token::TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");
    auto body = statement();
    if (increment != nullptr) {
        body = std::make_unique<Block>(vector<unique_ptr<Stmt>>{
            std::move(body), std::make_unique<Expression>(std::move(increment))
        });
    }
    if (condition == nullptr) condition = std::make_unique<Literal>(true);
    body = std::make_unique<_While>(std::move(condition), std::move(body));

    if (initializer != nullptr) {
        body = std::make_unique<Block>(vector<unique_ptr<Stmt>>{
            std::move(initializer),
            std::move(body)
        });
    }
    return body;
}

auto Parser::while_statement() -> unique_ptr<Stmt> {
    consume(Token::TokenType::LEFT_PAREN, "Expected '(' after \"while\"");
    auto condition = expression();
    consume(Token::TokenType::RIGHT_PAREN, "Expect ')' after while condition.");
    auto body = statement();
    return std::make_unique<_While>(std::move(condition), std::move(body));
}

auto Parser::if_statement() -> unique_ptr<Stmt> {
    consume(Token::TokenType::LEFT_PAREN, "Expected '(' after \"if\".");
    auto condition = expression();
    consume(Token::TokenType::RIGHT_PAREN, "Expect ')' after if condition.");
    auto then_branch = statement();
    std::unique_ptr<Stmt> else_branch;
    if (match(Token::TokenType::ELSE)) {
        else_branch = statement();
    }
    return std::make_unique<_If>(std::move(condition), std::move(then_branch), std::move(else_branch));
}

auto Parser::block() -> vector<unique_ptr<Stmt>> {
    auto statements = vector<unique_ptr<Stmt>>(); 
    while (!check(Token::TokenType::RIGHT_BRACE) && !at_end()) {
        statements.push_back(std::move(declaration()));
    }
    consume(Token::TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

auto Parser::print_statement() -> unique_ptr<Stmt> {
    auto value = expression();
    consume(Token::TokenType::SEMICOLON, "Expect a ';' after value.");
    return std::make_unique<Print>(std::move(value));
}

auto Parser::expression_statement() -> unique_ptr<Stmt> {
    auto expr = expression();
    consume(Token::TokenType::SEMICOLON, "Expect a ';' after expression.");
    return std::make_unique<Expression>(std::move(expr));
}

/*
    equality(), handles the grammar rules for operators such as "==" and "!=".
*/
auto Parser::equality() -> unique_ptr<Expr> {
    auto expr = comparison();
    while (match(Token::TokenType::BANG_EQUAL, Token::TokenType::EQUAL_EQUAL)) {
        auto op = previous();
        auto right = comparison();
        expr = std::make_unique<Binary>(
            Binary(std::move(expr), std::move(op), std::move(right)));
    }
    return expr;
}
/*
    comparison(), handles the grammar rules for operators such as ">", and "<=".
*/
auto Parser::comparison() -> unique_ptr<Expr> {
    auto expr = term();
    while (match(Token::TokenType::GREATER, Token::TokenType::GREATER_EQUAL,
                 Token::TokenType::LESS, Token::TokenType::LESS_EQUAL)) {
        auto op = previous();
        auto right = term();
        expr = std::make_unique<Binary>(
            Binary(std::move(expr), std::move(op), std::move(right)));
    }
    return expr;
}
/*
    term(), handles the grammar rules for infix arithmatic operators such as "+" and "-"
*/
auto Parser::term() -> unique_ptr<Expr> {
    auto expr = factor();
    while (match(Token::TokenType::MINUS, Token::TokenType::PLUS)) {
        auto op = previous();
        auto right = factor();
        expr = std::make_unique<Binary>(
            Binary(std::move(expr), std::move(op), std::move(right)));
    }
    return expr;
}
/*
    factor(), handles the grammar rules for multiplicative operators such as "*", and "/"
*/
auto Parser::factor() -> unique_ptr<Expr> {
    auto expr = unary();
    while (match(Token::TokenType::SLASH, Token::TokenType::STAR)) {
        auto op = previous();
        auto right = unary();
        expr = std::make_unique<Binary>(
            Binary(std::move(expr), std::move(op), std::move(right)));
    }
    return expr;
}
/*
    unary(), handles the grammar rules for prefix operators such as "!" and "-" <-- negation i.e -5
*/
auto Parser::unary() -> unique_ptr<Expr> {
    if (match(Token::TokenType::BANG, Token::TokenType::MINUS)) {
        auto op = previous();
        auto right = unary();
        return std::make_unique<Unary>(Unary(std::move(op), std::move(right)));
    }
    return primary();
}
/*
    primary(), handles grammar rules for literals and parenthesized expressions
*/
auto Parser::primary() -> unique_ptr<Expr> {
    if (match(Token::TokenType::FALSE))
        return std::make_unique<Literal>(false);
    if (match(Token::TokenType::TRUE))
        return std::make_unique<Literal>(true);
    if (match(Token::TokenType::NIL))
        return std::make_unique<Literal>(nullptr);
    if (match(Token::TokenType::NUMBER, Token::TokenType::STRING))
        return std::make_unique<Literal>(previous()->literal);
    if (match(Token::TokenType::IDENTIFIER))
        return std::make_unique<Variable>(previous());
    if (match(Token::TokenType::LEFT_PAREN)) {
        auto expr = expression();
        consume(Token::TokenType::RIGHT_PAREN, "Expect ')' after expression");
        return std::make_unique<Grouping>(std::move(expr));
    }
    std::cerr << "unexpected token: " << peek().ty << '\n';
    throw error(peek(), "Expect expression");
}

auto Parser::consume(Token::TokenType type, std::string message) -> unique_ptr<Token> {
    if (at_end()) throw error(peek(), "end of input.");
    if (check(type))
        return advance();
    throw error(peek(), message);
}

auto Parser::error(Token token, std::string message) -> ErrLog {
    sip_logger.error(token, message);
    return sip_logger;
}

auto Parser::synchronize() -> void {
    advance();
    while (!at_end()) {
        if (previous()->ty == Token::TokenType::SEMICOLON)
            return;
        switch (peek().ty) {
        case Token::TokenType::CLASS:
        case Token::TokenType::FUN:
        case Token::TokenType::VAR:
        case Token::TokenType::FOR:
        case Token::TokenType::IF:
        case Token::TokenType::WHILE:
        case Token::TokenType::PRINT:
        case Token::TokenType::RETURN:
            return;
        }
        advance();
    }
}

template <typename... T>
auto Parser::match(T... types) noexcept -> bool {
    return ((check(types) ? (Parser::advance(), true) : false) || ...);
}

auto Parser::check(Token::TokenType type) noexcept -> bool {
    if (Parser::at_end())
        return false;
    return Parser::peek().ty == type;
}

auto Parser::advance() noexcept -> unique_ptr<Token> {
    if (!Parser::at_end())
        ++current;
    return Parser::previous();
}

inline auto Parser::at_end() noexcept -> bool { return peek().ty == Token::TokenType::EOFF; }

[[nodiscard]]
inline auto Parser::peek() noexcept -> Token { return tokens[current]; }

inline auto Parser::previous() noexcept -> unique_ptr<Token> { return std::make_unique<Token>(tokens[current - 1]); }
