#include <Token.hpp>
#include <Expr.hpp>
#include <ErrLog.hpp>
#include <vector>

using std::unique_ptr;

class Parser {
public:
    Parser(std::vector<Token> tokens) : tokens(tokens) { std::make_unique<ErrLog>(sip_logger); } 

private:
    inline auto expression() -> unique_ptr<Expr> { return equality(); }

    auto equality() -> unique_ptr<Expr> {
        auto expr = comparison();
        while (match(Token::TokenType::BANG_EQUAL, Token::TokenType::EQUAL_EQUAL)) {
            auto op = previous();
            auto right = comparison();
            expr = std::make_unique<Binary>(Binary(std::move(expr), op, std::move(right)));
        }
        return expr;
    }

    auto comparison() -> unique_ptr<Expr> {
        auto expr = term();
        while (match(Token::TokenType::GREATER, Token::TokenType::GREATER_EQUAL, Token::TokenType::LESS, Token::TokenType::LESS_EQUAL)) {
            auto op = previous();
            auto right = term();
            expr = std::make_unique<Binary>(Binary(std::move(expr), op, std::move(right)));
        }
        return expr;
    }

    auto term() -> unique_ptr<Expr> {
        auto expr = factor();
        while (match(Token::TokenType::MINUS, Token::TokenType::PLUS)) {
            auto op = previous();
            auto right = factor();
            expr = std::make_unique<Binary>(Binary(std::move(expr), op, std::move(right)));
        }
        return expr;
    }

    auto factor() -> unique_ptr<Expr> {
        auto expr = unary();
        while (match(Token::TokenType::SLASH, Token::TokenType::STAR)) {
            auto op = previous();
            auto right = unary();
            expr = std::make_unique<Binary>(Binary(std::move(expr), op, std::move(right)));
        }
        return expr;
    }

    auto unary() -> unique_ptr<Expr> {
        if (match(Token::TokenType::BANG, Token::TokenType::MINUS)) {
            auto op = previous();
            auto right = unary();
            return std::make_unique<Unary>(Unary(op, std::move(right)));
        }
        return primary();
    }

    auto primary() -> unique_ptr<Expr> {
        if (match(Token::TokenType::FALSE)) return std::make_unique<Literal>(false);
        if (match(Token::TokenType::TRUE)) return std::make_unique<Literal>(true);
        if (match(Token::TokenType::NIL)) return std::make_unique<Literal>(nullptr);
        if (match(Token::TokenType::NUMBER, Token::TokenType::STRING)) return std::make_unique<Literal>(previous().literal); 
        if (match(Token::TokenType::LEFT_PAREN)) {
            auto expr = expression();
            consume(Token::TokenType::RIGHT_PAREN, "Expect ')' after expression");
            return std::make_unique<Grouping>(expr);
        }
    }

    auto consume(Token::TokenType type, std::string message) -> Token {
        if (check(type)) return advance();
        throw error(peek(), message);
    }

    auto error(Token token, std::string message) -> ErrLog {
        sip_logger.error(token, message);
        return sip_logger;
    } 

    template<typename... T>
    auto match(T... types) noexcept -> bool { return ((check(types) ? (Parser::advance(), true) : false) || ...); }

    auto check(Token::TokenType type) -> bool {
        if (Parser::at_end()) return false;
        return Parser::peek().ty == type;
    }

    auto advance() -> Token {
        if (!Parser::at_end()) ++current;
        return Parser::previous();
    }

    inline auto at_end() -> bool { return peek().ty == Token::TokenType::EOFF; }

    inline auto peek() -> Token { return tokens[current]; }

    inline auto previous() -> Token { return tokens[current - 1]; }

private: 
    std::vector<Token> tokens;    
    ErrLog sip_logger;
    int current = 0;
};
