#ifndef STMT
#define STMT
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

class _If;
class _While;
class Block;
class Expression;
class Function;
class Print;
class Var;
class Stmt;


class VisitorStmt {
public:
    virtual Object accept_If(_If &_if) = 0;

    virtual Object accept_While(_While &_while) = 0;

    virtual Object acceptBlock(Block &block) = 0;

    virtual Object acceptExpression(Expression &expression) = 0;

    virtual Object acceptFunction(Function &function) = 0;

    virtual Object acceptPrint(Print &print) = 0;

    virtual Object acceptVar(Var &var) = 0;

    virtual Object acceptStmt(Stmt &stmt) = 0;

};
class Stmt {
public:
    virtual Object visit(VisitorStmt &visitor) = 0;
};

class _If : public Stmt {
public:
    _If(shared_ptr<Expr> condition, shared_ptr<Stmt> then_branch, shared_ptr<Stmt> else_branch) : condition((condition)), then_branch((then_branch)), else_branch((else_branch)) {}

    Object visit (VisitorStmt &visitor) override {
        return visitor.accept_If(*this);
    }

    shared_ptr<Expr> condition;
    shared_ptr<Stmt> then_branch;
    shared_ptr<Stmt> else_branch;
};

class _While : public Stmt {
public:
    _While(shared_ptr<Expr> condition, shared_ptr<Stmt> body) : condition((condition)), body((body)) {}

    Object visit (VisitorStmt &visitor) override {
        return visitor.accept_While(*this);
    }

    shared_ptr<Expr> condition;
    shared_ptr<Stmt> body;
};

class Block : public Stmt {
public:
    Block(vector<shared_ptr<Stmt>> statements) : statements((statements)) {}

    Object visit (VisitorStmt &visitor) override {
        return visitor.acceptBlock(*this);
    }

    vector<shared_ptr<Stmt>> statements;
};

class Expression : public Stmt {
public:
    Expression(shared_ptr<Expr> expression) : expression((expression)) {}

    Object visit (VisitorStmt &visitor) override {
        return visitor.acceptExpression(*this);
    }

    shared_ptr<Expr> expression;
};

class Function : public Stmt {
public:
    Function(shared_ptr<Token> name, vector<shared_ptr<Token>> params, vector<shared_ptr<Stmt>> body) : name((name)), params((params)), body((body)) {}

    Object visit (VisitorStmt &visitor) override {
        return visitor.acceptFunction(*this);
    }

    shared_ptr<Token> name;
    vector<shared_ptr<Token>> params;
    vector<shared_ptr<Stmt>> body;
};

class Print : public Stmt {
public:
    Print(shared_ptr<Expr> expression) : expression((expression)) {}

    Object visit (VisitorStmt &visitor) override {
        return visitor.acceptPrint(*this);
    }

    shared_ptr<Expr> expression;
};

class Var : public Stmt {
public:
    Var(shared_ptr<Token> name, shared_ptr<Expr> initializer) : name((name)), initializer((initializer)) {}

    Object visit (VisitorStmt &visitor) override {
        return visitor.acceptVar(*this);
    }

    shared_ptr<Token> name;
    shared_ptr<Expr> initializer;
};


#endif // STMT
