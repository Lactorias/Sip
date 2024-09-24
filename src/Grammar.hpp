#include "./Token.hpp"
#include <memory>

class Expr {
public:
    virtual ~Expr() = default; 
};

class Binary : public Expr {
public:
    Binary(std::shared_ptr<Expr> left, std::shared_ptr<Token> operatorToken, std::shared_ptr<Expr> right)
        : left(std::move(left)), 
        operatorToken(std::move(operatorToken)), 
        right(std::move(right)) {}
    std::shared_ptr<Expr> left;
    std::shared_ptr<Token> operatorToken;
    std::shared_ptr<Expr> right;
};
