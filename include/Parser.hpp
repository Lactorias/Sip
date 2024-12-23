#ifndef PARSER
#define PARSER
#include <Token.hpp>
#include <Expr.hpp>
#include <ErrLog.hpp>
#include <Stmt.hpp>
#include <vector>


class Parser {
public:
    Parser(std::vector<Token> tokens) : tokens(tokens) { std::make_shared<ErrLog>(sip_logger); } 
    
    auto parse() -> std::vector<std::shared_ptr<Stmt>>;

private:
    auto statement() -> std::shared_ptr<Stmt>;

    auto function(std::string kind) -> std::shared_ptr<Function>;

    auto call() -> shared_ptr<Expr>;

    auto finish_call(shared_ptr<Expr> callee) -> shared_ptr<Expr>; 

    auto assignment() -> shared_ptr<Expr>;

    auto block() -> std::vector<std::shared_ptr<Stmt>>;

    auto declaration() -> std::shared_ptr<Stmt>;

    auto _or() -> shared_ptr<Expr>;

    auto _and() -> shared_ptr<Expr>;

    auto while_statement() -> shared_ptr<Stmt>;

    auto for_statement() -> std::shared_ptr<Stmt>;

    auto var_declaration() -> shared_ptr<Stmt>;

    auto print_statement() -> shared_ptr<Stmt>;

    auto if_statement() -> std::shared_ptr<Stmt>;

    auto expression_statement() -> shared_ptr<Stmt>;

    inline auto expression() -> shared_ptr<Expr>;

    auto equality() -> shared_ptr<Expr>;

    auto comparison() -> shared_ptr<Expr>;

    auto term() -> shared_ptr<Expr>;

    auto factor() -> shared_ptr<Expr>;

    auto unary() -> shared_ptr<Expr>;

    auto primary() -> shared_ptr<Expr>;

    auto consume(Token::TokenType type, std::string message) -> shared_ptr<Token>;

    auto error(Token token, std::string message) -> ErrLog;

    auto synchronize() -> void;

    template<typename... T>
    auto match(T... types) noexcept -> bool;

    auto check(Token::TokenType type) noexcept -> bool;

    auto advance() noexcept -> shared_ptr<Token>;

    inline auto at_end() noexcept -> bool;

    inline auto peek() noexcept -> Token;

    inline auto previous() noexcept -> shared_ptr<Token>;

private: 
    std::vector<Token> tokens;    
    ErrLog sip_logger;
    int current = 0;
};
#endif // PARSER
