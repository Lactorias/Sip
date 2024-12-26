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


inline static int EXPR_ID = 0;
class Assign;
class Binary;
class Call;
class Get;
class Grouping;
class Literal;
class Logical;
class Set;
class _This;
class Unary;
class Variable;
class Expr;


class VisitorExpr {
public:
    virtual Object acceptAssign(Assign &assign) = 0;

    virtual Object acceptBinary(Binary &binary) = 0;

    virtual Object acceptCall(Call &call) = 0;

    virtual Object acceptGet(Get &get) = 0;

    virtual Object acceptGrouping(Grouping &grouping) = 0;

    virtual Object acceptLiteral(const Literal &literal) = 0;

    virtual Object acceptLogical(Logical &logical) = 0;

    virtual Object acceptSet(Set &set) = 0;

    virtual Object accept_This(_This &_this) = 0;

    virtual Object acceptUnary(Unary &unary) = 0;

    virtual Object acceptVariable(Variable &variable) = 0;

    virtual Object acceptExpr(Expr &expr) = 0;

};
class Expr {
public:
    virtual Object visit(VisitorExpr &visitor) = 0;
    virtual int get_id() const = 0;
};

class Assign : public Expr {
public:
    Assign(shared_ptr<Token> name, shared_ptr<Expr> value) : name(std::move(name)), value(std::move(value)), this_id(EXPR_ID++) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptAssign(*this);
    }



    int get_id() const override {
        return this_id;
    }

    int this_id;
    shared_ptr<Token> name;
    shared_ptr<Expr> value;
};

class Binary : public Expr {
public:
    Binary(shared_ptr<Expr> left, unique_ptr<Token> oper, shared_ptr<Expr> right) : left(std::move(left)), oper(std::move(oper)), right(std::move(right)), this_id(EXPR_ID++) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptBinary(*this);
    }



    int get_id() const override {
        return this_id;
    }

    int this_id;
    shared_ptr<Expr> left;
    unique_ptr<Token> oper;
    shared_ptr<Expr> right;
};

class Call : public Expr {
public:
    Call(shared_ptr<Expr> callee, shared_ptr<Token> paren, vector<shared_ptr<Expr>> arguments) : callee(std::move(callee)), paren(std::move(paren)), arguments(std::move(arguments)), this_id(EXPR_ID++) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptCall(*this);
    }



    int get_id() const override {
        return this_id;
    }

    int this_id;
    shared_ptr<Expr> callee;
    shared_ptr<Token> paren;
    vector<shared_ptr<Expr>> arguments;
};

class Get : public Expr {
public:
    Get(unique_ptr<Expr> object, unique_ptr<Token> name) : object(std::move(object)), name(std::move(name)), this_id(EXPR_ID++) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptGet(*this);
    }



    int get_id() const override {
        return this_id;
    }

    int this_id;
    unique_ptr<Expr> object;
    unique_ptr<Token> name;
};

class Grouping : public Expr {
public:
    Grouping(shared_ptr<Expr> expression) : expression(std::move(expression)), this_id(EXPR_ID++) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptGrouping(*this);
    }



    int get_id() const override {
        return this_id;
    }

    int this_id;
    shared_ptr<Expr> expression;
};

class Literal : public Expr {
public:
    Literal(const Object value) : value(std::move(value)), this_id(EXPR_ID++) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptLiteral(*this);
    }



    int get_id() const override {
        return this_id;
    }

    int this_id;
    const Object value;
};

class Logical : public Expr {
public:
    Logical(shared_ptr<Expr> left, unique_ptr<Token> oper, shared_ptr<Expr> right) : left(std::move(left)), oper(std::move(oper)), right(std::move(right)), this_id(EXPR_ID++) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptLogical(*this);
    }



    int get_id() const override {
        return this_id;
    }

    int this_id;
    shared_ptr<Expr> left;
    unique_ptr<Token> oper;
    shared_ptr<Expr> right;
};

class Set : public Expr {
public:
    Set(shared_ptr<Expr> object, unique_ptr<Token> name, shared_ptr<Expr> value) : object(std::move(object)), name(std::move(name)), value(std::move(value)), this_id(EXPR_ID++) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptSet(*this);
    }



    int get_id() const override {
        return this_id;
    }

    int this_id;
    shared_ptr<Expr> object;
    unique_ptr<Token> name;
    shared_ptr<Expr> value;
};

class _This : public Expr {
public:
    _This(unique_ptr<Token> keyword) : keyword(std::move(keyword)), this_id(EXPR_ID++) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.accept_This(*this);
    }



    int get_id() const override {
        return this_id;
    }

    int this_id;
    unique_ptr<Token> keyword;
};

class Unary : public Expr {
public:
    Unary(unique_ptr<Token> oper, shared_ptr<Expr> right) : oper(std::move(oper)), right(std::move(right)), this_id(EXPR_ID++) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptUnary(*this);
    }



    int get_id() const override {
        return this_id;
    }

    int this_id;
    unique_ptr<Token> oper;
    shared_ptr<Expr> right;
};

class Variable : public Expr {
public:
    Variable(unique_ptr<Token> name) : name(std::move(name)), this_id(EXPR_ID++) {}

    Object visit (VisitorExpr &visitor) override {
        return visitor.acceptVariable(*this);
    }



    int get_id() const override {
        return this_id;
    }

    int this_id;
    unique_ptr<Token> name;
};


#endif // EXPR
