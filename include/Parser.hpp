#ifndef PARSER
#define PARSER
#include <Token.hpp>
#include <Expr.hpp>
#include <ErrLog.hpp>
#include <vector>

using std::unique_ptr;

class Parser {
public:
    Parser(std::vector<Token> tokens) : tokens(tokens) { std::make_unique<ErrLog>(sip_logger); } 
    
    auto parse() -> unique_ptr<Expr>;

private:
    inline auto expression() -> unique_ptr<Expr>;

    auto equality() -> unique_ptr<Expr>;

    auto comparison() -> unique_ptr<Expr>;

    auto term() -> unique_ptr<Expr>;

    auto factor() -> unique_ptr<Expr>;

    auto unary() -> unique_ptr<Expr>;

    auto primary() -> unique_ptr<Expr>;

    auto consume(Token::TokenType type, std::string message) -> Token;

    auto error(Token token, std::string message) -> ErrLog;

    auto synchronize() -> void;

    template<typename... T>
    auto match(T... types) noexcept -> bool;

    auto check(Token::TokenType type) -> bool;

    auto advance() -> Token;

    inline auto at_end() -> bool;

    inline auto peek() -> Token;

    inline auto previous() -> Token;

private: 
    std::vector<Token> tokens;    
    ErrLog sip_logger;
    int current = 0;
};
#endif // PARSER
