#include <Token.hpp>
#include <cstdint>
#include <memory>
#include <utility>
#include <variant>
#include <any>

using Object = std::variant<std::monostate, int, double, std::string>;
using std::unique_ptr;

class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor {
public:
    virtual Object acceptBinary(Binary &binary) = 0;

    virtual Object acceptGrouping(Grouping &grouping) = 0;

    virtual Object acceptLiteral(const Literal &literal) = 0;

    virtual Object acceptUnary(Unary &unary) = 0;
};

class Expr {
public:
    virtual Object visit(Visitor &visitor) = 0;
};

class Binary : public Expr {
public:
    Binary(unique_ptr<Expr> left, const Token &oper, unique_ptr<Expr> right)
        : left(std::move(left)), oper(oper), right(std::move(right)) {}

    Object visit(Visitor &visitor) override {
        return visitor.acceptBinary(*this);
    }

    // unique_ptr
    // take the ref only
    unique_ptr<Expr> left;
    const Token &oper;
    unique_ptr<Expr> right;
};

class Grouping : public Expr {
public:
    Grouping(Expr &expression) : expression(expression) {}
    Object visit(Visitor &visitor) override {
        return visitor.acceptGrouping(*this);
    }
    Expr &expression;
};

class Literal : public Expr {
public:
    Literal(Object value) : value(value) {}
    Object visit(Visitor &visitor) override {
        return visitor.acceptLiteral(*this);
    }
    const Object value;
};

class Unary : public Expr {
public:
    Unary(const Token oper, unique_ptr<Expr> &&right)
        : oper(oper), right(std::move(right)) {}
    Object visit(Visitor &visitor) override { return visitor.acceptUnary(*this); }
    const Token oper;
    unique_ptr<Expr> right;
};
