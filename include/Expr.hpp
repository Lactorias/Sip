#ifndef EXPR
#define EXPR
#include <Token.hpp>
#include <Expr.hpp>
#include <variant>
#include <memory>
#include <utility>
using Object = std::variant<std::monostate, int, std::string, double, bool>;
using std::unique_ptr;

class Binary;
class Grouping;
class Literal;
class Unary;
class Expr;


class Visitor {
public:
    virtual Object acceptBinary(Binary &binary) = 0;

    virtual Object acceptGrouping(Grouping &grouping) = 0;

    virtual Object acceptLiteral(Literal &literal) = 0;

    virtual Object acceptUnary(Unary &unary) = 0;

    virtual Object acceptExpr(Expr &expr) = 0;

};
class Expr {
public:
    virtual Object visit(Visitor &visitor) = 0;
};

class Binary : public Expr {
public:
    Binary(unique_ptr<Expr> left, unique_ptr<Token> oper, unique_ptr<Expr> right) : left(std::move(left)), oper(std::move(oper)), right(std::move(right)) {}

    Object visit(Visitor &visitor) override {
        return visitor.acceptBinary(*this);
    }

    unique_ptr<Expr> left;
    unique_ptr<Token> oper;
    unique_ptr<Expr> right;
};

class Grouping : public Expr {
public:
    Grouping(unique_ptr<Expr> expression) : expression(std::move(expression)) {}

    Object visit(Visitor &visitor) override {
        return visitor.acceptGrouping(*this);
    }

    unique_ptr<Expr> expression;
};

class Literal : public Expr {
public:
    Literal(const Object value) : value(std::move(value)) {}

    Object visit(Visitor &visitor) override {
        return visitor.acceptLiteral(*this);
    }

    const Object value;
};

class Unary : public Expr {
public:
    Unary(unique_ptr<Token> oper, unique_ptr<Expr> right) : oper(std::move(oper)), right(std::move(right)) {}

    Object visit(Visitor &visitor) override {
        return visitor.acceptUnary(*this);
    }

    unique_ptr<Token> oper;
    unique_ptr<Expr> right;
};


#endif // EXPR
