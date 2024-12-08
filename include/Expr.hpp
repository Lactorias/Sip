#ifndef EXPR
#define EXPR
#include <Token.hpp>
#include <cstdint>
#include <memory>
#include <utility>
#include <variant>
#include <any>

using Object = std::variant<std::monostate, int, std::string, double, bool>;
using std::unique_ptr;

class Binary;
class Grouping;
class Literal;
class Unary;
class Expr;
/*
    Visitor : Base class, make a visitor for one job, i.e to print types, call visit on type with
              a visitor parameter to go to it and call accept to perform the work.
*/
class Visitor {
public:
    virtual Object acceptExpr(Expr &expr) = 0;

    virtual Object acceptBinary(Binary &binary) = 0;

    virtual Object acceptGrouping(Grouping &grouping) = 0;

    virtual Object acceptLiteral(const Literal &literal) = 0;

    virtual Object acceptUnary(Unary &unary) = 0;
};
/*
    Expr : Highest level of our syntactic grammar, heres why,
    Expr : Binary | Unary | Grouping | Literal
    It can go to any of its derived type, has a all non-terminal body in terms of grammar.
*/
class Expr {
public:
    virtual Object visit(Visitor &visitor) = 0;
};
/*
    Binary : Handles the infix arithmatic operators and logic operators, i.e LHS op RHS
*/
class Binary : public Expr {
public:
    Binary(unique_ptr<Expr> left, unique_ptr<Token> oper, unique_ptr<Expr> right)
        : left(std::move(left)), oper(std::move(oper)), right(std::move(right)) {}

    Object visit(Visitor &visitor) override {
        return visitor.acceptBinary(*this);
    }

    // unique_ptr
    // take the ref only
    unique_ptr<Expr> left;
    unique_ptr<Token> oper;
    unique_ptr<Expr> right;
};
/*
    Grouping : Parenthesis expressions, a pair of "( expression )" around an expression.
*/
class Grouping : public Expr {
public:
    Grouping(unique_ptr<Expr> expression) : expression(std::move(expression)) {}
    Object visit(Visitor &visitor) override {
        return visitor.acceptGrouping(*this);
    }
    unique_ptr<Expr> expression;
};

/*
    Literal : numbers, strings, booleans, null.
*/
class Literal : public Expr {
public:
    Literal(const Object value) : value(value) {}
    Object visit(Visitor &visitor) override {
        return visitor.acceptLiteral(*this);
    }
    const Object value;
};
/*
    Unary : Prefix operators like '!' to perform logical nots, or '-' to negate a number.
*/
class Unary : public Expr {
public:
    Unary(unique_ptr<Token> oper, unique_ptr<Expr> &&right)
        : oper(std::move(oper)), right(std::move(right)) {}
    Object visit(Visitor &visitor) override { return visitor.acceptUnary(*this); }
    unique_ptr<Token> oper;
    unique_ptr<Expr> right;
};

#endif // EXPR