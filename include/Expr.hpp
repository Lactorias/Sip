#ifndef EXPR
#define EXPR
#include <SipVariant.hpp>
#include <Token.hpp>
#include <Expr.hpp>
#include <variant>
#include <vector>
#include <memory>
#include <LoxCallable.hpp>
#include <utility>
using std::unique_ptr;
using std::shared_ptr;
using std::vector;

class Assign;
class Binary;
class Call;
class Grouping;
class Literal;
class Logical;
class Unary;
class Variable;
class Expr;


class VisitorExpr {
public:
    virtual Object acceptAssign(Assign &assign) = 0;

    virtual Object acceptBinary(Binary &binary) = 0;

    virtual Object acceptCall(Call &call) = 0;

    virtual Object acceptGrouping(Grouping &grouping) = 0;

    virtual Object acceptLiteral(const Literal &literal) = 0;

    virtual Object acceptLogical(Logical &logical) = 0;

    virtual Object acceptUnary(Unary &unary) = 0;

    virtual Object acceptVariable(Variable &variable) = 0;

    virtual Object acceptExpr(Expr &expr) = 0;

};
class Expr {
public:
    virtual Object visit(VisitorExpr &visitor) = 0;
};

class Assign : public Expr {
public:
    Assign(unique_ptr<Token> name, unique_ptr<Expr> value) : name(std::move(name)), value(std::move(value)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptAssign(*this);
    }

    unique_ptr<Token> name;
    unique_ptr<Expr> value;
};

class Binary : public Expr {
public:
    Binary(unique_ptr<Expr> left, unique_ptr<Token> oper, unique_ptr<Expr> right) : left(std::move(left)), oper(std::move(oper)), right(std::move(right)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptBinary(*this);
    }

    unique_ptr<Expr> left;
    unique_ptr<Token> oper;
    unique_ptr<Expr> right;
};

class Call : public Expr {
public:
    Call(shared_ptr<Expr> callee, shared_ptr<Token> paren, vector<shared_ptr<Expr>> arguments) : callee(std::move(callee)), paren(std::move(paren)), arguments(std::move(arguments)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptCall(*this);
    }

    shared_ptr<Expr> callee;
    shared_ptr<Token> paren;
    vector<shared_ptr<Expr>> arguments;
};

class Grouping : public Expr {
public:
    Grouping(unique_ptr<Expr> expression) : expression(std::move(expression)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptGrouping(*this);
    }

    unique_ptr<Expr> expression;
};

class Literal : public Expr {
public:
    Literal(const Object value) : value(std::move(value)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptLiteral(*this);
    }

    const Object value;
};

class Logical : public Expr {
public:
    Logical(unique_ptr<Expr> left, unique_ptr<Token> oper, unique_ptr<Expr> right) : left(std::move(left)), oper(std::move(oper)), right(std::move(right)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptLogical(*this);
    }

    unique_ptr<Expr> left;
    unique_ptr<Token> oper;
    unique_ptr<Expr> right;
};

class Unary : public Expr {
public:
    Unary(unique_ptr<Token> oper, unique_ptr<Expr> right) : oper(std::move(oper)), right(std::move(right)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptUnary(*this);
    }

    unique_ptr<Token> oper;
    unique_ptr<Expr> right;
};

class Variable : public Expr {
public:
    Variable(unique_ptr<Token> name) : name(std::move(name)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptVariable(*this);
    }

    unique_ptr<Token> name;
};


#endif // EXPR
