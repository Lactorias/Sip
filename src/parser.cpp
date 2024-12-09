#include <ErrLog.hpp>
#include <Expr.hpp>
#include <Token.hpp>
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
    return equality(); 
}

/*
    parse(), kicks off our parsing, starting at the 'lowest' level "expression".    
*/
auto Parser::parse() -> unique_ptr<Expr>{
    return expression();
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
    if (match(Token::TokenType::LEFT_PAREN)) {
        auto expr = expression();
        consume(Token::TokenType::RIGHT_PAREN, "Expect ')' after expression");
        return std::make_unique<Grouping>(std::move(expr));
    }
    throw error(peek(), "Expect expression");
}

auto Parser::consume(Token::TokenType type, std::string message) -> unique_ptr<Token> {
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

inline auto Parser::peek() noexcept -> Token { return tokens[current]; }

inline auto Parser::previous() noexcept -> unique_ptr<Token> { return std::make_unique<Token>(tokens[current - 1]); }
