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
inline auto Parser::expression() -> shared_ptr<Expr> {
    return assignment(); 
}

auto Parser::assignment() -> shared_ptr<Expr> {
    auto expr = _or();
    if (match(Token::TokenType::EQUAL)) {
        auto equals = previous();
        auto value = assignment();
        if (auto var_expr = dynamic_cast<Variable*>(expr.get())) {
            auto name = (*var_expr).name;
            return std::make_shared<Assign>((name), (value));
        }
        error(*equals, "Invalid assignment target.");
    }
    return expr;
}

auto Parser::_or() -> shared_ptr<Expr> {
    auto expr = _and();
    while (match(Token::TokenType::OR)) {
        auto op = previous();
        auto right = _and();
        expr = std::make_shared<Logical>((expr), (op), (right));
    }
    return expr;
}

auto Parser::_and() -> shared_ptr<Expr> {
    auto expr = equality();
    while (match(Token::TokenType::AND)) {
        auto op = previous();
        auto right = equality();
        expr = std::make_shared<Logical>((expr), (op), (right));
    }
    return expr;
}

auto Parser::function(std::string kind) -> shared_ptr<Function> {
    auto name = consume(Token::TokenType::IDENTIFIER, "Expect " + kind + " name.");
    consume(Token::TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");
    auto parameters = vector<shared_ptr<Token>>();
    if (!check(Token::TokenType::RIGHT_PAREN)) {
        std::cout << "Found parameters to parse..." << '\n';
        do {
            std::cout << "Parsing parameters..." << '\n';
            auto param = consume(Token::TokenType::IDENTIFIER, "Expect parameter name.");
            std::cout << "Parameter consumed!" << '\n';
            parameters.push_back(param);
        } while (match(Token::TokenType::COMMA));
    }
    consume(Token::TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
    consume(Token::TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
    auto body = block();
    // maybe make the name shared_ptr
    std::cout << "hello......................" << '\n';
    return std::make_shared<Function>((name), parameters, body);
}

/*
    parse(), kicks off our parsing, starting at the 'lowest' level "expression".    
*/
auto Parser::parse() -> std::vector<std::shared_ptr<Stmt>> {
    auto statements = std::vector<std::shared_ptr<Stmt>>();
    while (!at_end()) statements.push_back(declaration());
    return statements;
}

auto Parser::declaration() -> std::shared_ptr<Stmt> {
    try {
        if (match(Token::TokenType::FUN)) return function("function");
        if (match(Token::TokenType::VAR)) return var_declaration();
        return statement();
    } catch (ErrLog error) {
        std::cout << "lol i bugged" << '\n';
        synchronize();
        return nullptr;
    }
}

auto Parser::var_declaration() -> shared_ptr<Stmt> {
    auto name = consume(Token::TokenType::IDENTIFIER, "Expect variable name.");
    shared_ptr<Expr> initializer;
    if (match(Token::TokenType::EQUAL)) initializer = expression();
    consume(Token::TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<Var>((name), (initializer));
}

auto Parser::statement() -> std::shared_ptr<Stmt> {
    if (match(Token::TokenType::FOR)) return for_statement();
    if (match(Token::TokenType::IF)) return if_statement();
    if (match(Token::TokenType::PRINT)) return print_statement();
    if (match(Token::TokenType::WHILE)) return while_statement();
    if (match(Token::TokenType::LEFT_BRACE)) return std::make_shared<Block>(block());
    return expression_statement();
}

auto Parser::for_statement() -> std::shared_ptr<Stmt> {
    consume(Token::TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

    // parse initializer
    std::shared_ptr<Stmt> initializer;
    if (match(Token::TokenType::SEMICOLON)) {
        initializer = nullptr;
    } else if (match(Token::TokenType::VAR)) {
        initializer = var_declaration();
    } else {
        initializer = expression_statement();
    }

    // parse condition
    std::shared_ptr<Expr> condition = nullptr;
    if (!check(Token::TokenType::SEMICOLON)) {
        condition = expression();
    }
    consume(Token::TokenType::SEMICOLON, "Expect ';' after loop condiiton.");

    std::shared_ptr<Expr> increment = nullptr;
    if (!check(Token::TokenType::RIGHT_PAREN)) {
        increment = expression();
    }
    consume(Token::TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

    auto body = statement();
    if (increment != nullptr) {
        body = std::make_shared<Block>(vector<std::shared_ptr<Stmt>>{
            body, std::make_shared<Expression>(increment)
        });
    }
    if (condition == nullptr) condition = std::make_shared<Literal>(true);
    body = std::make_shared<_While>(condition, body);

    if (initializer != nullptr) {
        body = std::make_shared<Block>(vector<std::shared_ptr<Stmt>>{
            initializer, body
        });
    }
    return body;
}

auto Parser::while_statement() -> shared_ptr<Stmt> {
    consume(Token::TokenType::LEFT_PAREN, "Expected '(' after \"while\"");
    auto condition = expression();
    consume(Token::TokenType::RIGHT_PAREN, "Expect ')' after while condition.");
    auto body = statement();
    return std::make_shared<_While>((condition), (body));
}

auto Parser::if_statement() -> std::shared_ptr<Stmt> {
    consume(Token::TokenType::LEFT_PAREN, "Expected '(' after \"if\".");
    auto condition = expression();
    consume(Token::TokenType::RIGHT_PAREN, "Expect ')' after if condition.");
    auto then_branch = statement();
    std::shared_ptr<Stmt> else_branch;
    if (match(Token::TokenType::ELSE)) {
        else_branch = statement();
    }
    return std::make_shared<_If>((condition), then_branch, else_branch);
}

auto Parser::block() -> vector<std::shared_ptr<Stmt>> {
    auto statements = vector<std::shared_ptr<Stmt>>(); 
    while (!check(Token::TokenType::RIGHT_BRACE) && !at_end()) {
        statements.push_back(declaration());
    }
    consume(Token::TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

auto Parser::print_statement() -> shared_ptr<Stmt> {
    auto value = expression();
    consume(Token::TokenType::SEMICOLON, "Expect a ';' after value.");
    return std::make_shared<Print>((value));
}

auto Parser::expression_statement() -> shared_ptr<Stmt> {
    auto expr = expression();
    consume(Token::TokenType::SEMICOLON, "Expect a ';' after expression.");
    return std::make_shared<Expression>((expr));
}

/*
    equality(), handles the grammar rules for operators such as "==" and "!=".
*/
auto Parser::equality() -> shared_ptr<Expr> {
    auto expr = comparison();
    while (match(Token::TokenType::BANG_EQUAL, Token::TokenType::EQUAL_EQUAL)) {
        auto op = previous();
        auto right = comparison();
        expr = std::make_shared<Binary>(
            Binary((expr), (op), (right)));
    }
    return expr;
}
/*
    comparison(), handles the grammar rules for operators such as ">", and "<=".
*/
auto Parser::comparison() -> shared_ptr<Expr> {
    auto expr = term();
    while (match(Token::TokenType::GREATER, Token::TokenType::GREATER_EQUAL,
                 Token::TokenType::LESS, Token::TokenType::LESS_EQUAL)) {
        auto op = previous();
        auto right = term();
        expr = std::make_shared<Binary>(
            Binary((expr), (op), (right)));
    }
    return expr;
}
/*
    term(), handles the grammar rules for infix arithmatic operators such as "+" and "-"
*/
auto Parser::term() -> shared_ptr<Expr> {
    auto expr = factor();
    while (match(Token::TokenType::MINUS, Token::TokenType::PLUS)) {
        auto op = previous();
        auto right = factor();
        expr = std::make_shared<Binary>(
            Binary((expr), (op), (right)));
    }
    return expr;
}
/*
    factor(), handles the grammar rules for multiplicative operators such as "*", and "/"
*/
auto Parser::factor() -> shared_ptr<Expr> {
    auto expr = unary();
    while (match(Token::TokenType::SLASH, Token::TokenType::STAR)) {
        auto op = previous();
        auto right = unary();
        expr = std::make_shared<Binary>(
            Binary((expr), (op), (right)));
    }
    return expr;
}
/*
    unary(), handles the grammar rules for prefix operators such as "!" and "-" <-- negation i.e -5
*/
auto Parser::unary() -> shared_ptr<Expr> {
    if (match(Token::TokenType::BANG, Token::TokenType::MINUS)) {
        auto op = previous();
        auto right = unary();
        return std::make_shared<Unary>((op), (right));
    }
    return call();
}

auto Parser::call() -> shared_ptr<Expr> {
    auto expr = primary();

    while (true) {
        if (match(Token::TokenType::LEFT_PAREN)) {
            std::cout << "to finish the cal ahahahah" << '\n';
            expr = finish_call((expr));
        } else {
            break;
        }
    }
    return expr;
}

auto Parser::finish_call(shared_ptr<Expr> callee) -> shared_ptr<Expr> {
    vector<shared_ptr<Expr>> arguments;
    if (!check(Token::TokenType::RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                error(peek(), "Can't have more than 255 arguments.");
            }
            arguments.push_back(expression());
        } while (match(Token::TokenType::COMMA));
    }
    std::cout << "ahhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh " << '\n';
    auto paren = consume(Token::TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
    return std::make_shared<Call>((callee), (paren), (arguments));
}


/*
    primary(), handles grammar rules for literals and parenthesized expressions
*/
auto Parser::primary() -> shared_ptr<Expr> {
    if (match(Token::TokenType::FALSE))
        return std::make_shared<Literal>(false);
    if (match(Token::TokenType::TRUE))
        return std::make_shared<Literal>(true);
    if (match(Token::TokenType::NIL))
        return std::make_shared<Literal>(nullptr);
    if (match(Token::TokenType::NUMBER, Token::TokenType::STRING))
        return std::make_shared<Literal>(previous()->literal);
    if (match(Token::TokenType::IDENTIFIER)) {
        std::cout << "identifier triggered" << '\n';
        return std::make_shared<Variable>(previous());
    }
    if (match(Token::TokenType::LEFT_PAREN)) {
        auto expr = expression();
        consume(Token::TokenType::RIGHT_PAREN, "Expect ')' after expression");
        return std::make_shared<Grouping>((expr));
    }
    std::cerr << "unexpected token: " << peek().ty << '\n';
    throw error(peek(), "Expect expression");
}

auto Parser::consume(Token::TokenType type, std::string message) -> shared_ptr<Token> {
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

auto Parser::advance() noexcept -> shared_ptr<Token> {
    if (!Parser::at_end())
        ++current;
    return Parser::previous();
}

inline auto Parser::at_end() noexcept -> bool { return peek().ty == Token::TokenType::EOFF; }

[[nodiscard]]
inline auto Parser::peek() noexcept -> Token { return tokens[current]; }

inline auto Parser::previous() noexcept -> shared_ptr<Token> { return std::make_shared<Token>(tokens[current - 1]); }
