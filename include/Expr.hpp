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
    Assign(shared_ptr<Token> name, shared_ptr<Expr> value) : name((name)), value((value)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptAssign(*this);
    }

    shared_ptr<Token> name;
    shared_ptr<Expr> value;
};

class Binary : public Expr {
public:
    Binary(shared_ptr<Expr> left, shared_ptr<Token> oper, shared_ptr<Expr> right) : left((left)), oper((oper)), right((right)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptBinary(*this);
    }

    shared_ptr<Expr> left;
    shared_ptr<Token> oper;
    shared_ptr<Expr> right;
};

class Call : public Expr {
public:
    Call(shared_ptr<Expr> callee, shared_ptr<Token> paren, vector<shared_ptr<Expr>> arguments) : callee((callee)), paren((paren)), arguments((arguments)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptCall(*this);
    }

    shared_ptr<Expr> callee;
    shared_ptr<Token> paren;
    vector<shared_ptr<Expr>> arguments;
};

class Grouping : public Expr {
public:
    Grouping(shared_ptr<Expr> expression) : expression((expression)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptGrouping(*this);
    }

    shared_ptr<Expr> expression;
};

class Literal : public Expr {
public:
    Literal(const Object value) : value((value)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptLiteral(*this);
    }

    const Object value;
};

class Logical : public Expr {
public:
    Logical(shared_ptr<Expr> left, shared_ptr<Token> oper, shared_ptr<Expr> right) : left((left)), oper((oper)), right((right)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptLogical(*this);
    }

    shared_ptr<Expr> left;
    shared_ptr<Token> oper;
    shared_ptr<Expr> right;
};

class Unary : public Expr {
public:
    Unary(shared_ptr<Token> oper, shared_ptr<Expr> right) : oper((oper)), right((right)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptUnary(*this);
    }

    shared_ptr<Token> oper;
    shared_ptr<Expr> right;
};

class Variable : public Expr {
public:
    Variable(shared_ptr<Token> name) : name((name)) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptVariable(*this);
    }

    shared_ptr<Token> name;
};


#endif // EXPR
