#ifndef PARSER
#define PARSER
#include <Token.hpp>
#include <Expr.hpp>
#include <ErrLog.hpp>
#include <Stmt.hpp>
#include <vector>


class Parser {
public:
    Parser(std::vector<Token> tokens) : tokens(tokens) { std::make_unique<ErrLog>(sip_logger); } 
    
    auto parse() -> std::vector<std::shared_ptr<Stmt>>;

private:
    auto statement() -> std::shared_ptr<Stmt>;

    auto assignment() -> unique_ptr<Expr>;

    auto block() -> std::vector<std::shared_ptr<Stmt>>;

    auto declaration() -> std::shared_ptr<Stmt>;

    auto _or() -> unique_ptr<Expr>;

    auto _and() -> unique_ptr<Expr>;

    auto while_statement() -> unique_ptr<Stmt>;

    auto for_statement() -> std::shared_ptr<Stmt>;

    auto var_declaration() -> unique_ptr<Stmt>;

    auto print_statement() -> unique_ptr<Stmt>;

    auto if_statement() -> std::shared_ptr<Stmt>;

    auto expression_statement() -> unique_ptr<Stmt>;

    inline auto expression() -> unique_ptr<Expr>;

    auto equality() -> unique_ptr<Expr>;

    auto comparison() -> unique_ptr<Expr>;

    auto term() -> unique_ptr<Expr>;

    auto factor() -> unique_ptr<Expr>;

    auto unary() -> unique_ptr<Expr>;

    auto primary() -> unique_ptr<Expr>;

    auto consume(Token::TokenType type, std::string message) -> unique_ptr<Token>;

    auto error(Token token, std::string message) -> ErrLog;

    auto synchronize() -> void;

    template<typename... T>
    auto match(T... types) noexcept -> bool;

    auto check(Token::TokenType type) noexcept -> bool;

    auto advance() noexcept -> unique_ptr<Token>;

    inline auto at_end() noexcept -> bool;

    inline auto peek() noexcept -> Token;

    inline auto previous() noexcept -> unique_ptr<Token>;

private: 
    std::vector<Token> tokens;    
    ErrLog sip_logger;
    int current = 0;
};
#endif // PARSER
