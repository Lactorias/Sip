#ifndef STMT
#define STMT
#include <Token.hpp>
#include <Expr.hpp>
#include <variant>
#include <memory>
#include <utility>
using Object = std::variant<std::monostate, int, std::string, double, bool>;
using std::unique_ptr;
using std::vector;

class Block;
class Expression;
class Print;
class Var;
class Stmt;


class VisitorStmt {
public:
    virtual Object acceptBlock(Block &block) = 0;

    virtual Object acceptExpression(Expression &expression) = 0;

    virtual Object acceptPrint(Print &print) = 0;

    virtual Object acceptVar(Var &var) = 0;

    virtual Object acceptStmt(Stmt &stmt) = 0;

};
class Stmt {
public:
    virtual Object visit(VisitorStmt &visitor) = 0;
};

class Block : public Stmt {
public:
    Block(vector<unique_ptr<Stmt>> statements) : statements(std::move(statements)) {}

    Object visit (VisitorStmt &visitor) override {
        return visitor.acceptBlock(*this);
    }

    vector<unique_ptr<Stmt>> statements;
};

class Expression : public Stmt {
public:
    Expression(unique_ptr<Expr> expression) : expression(std::move(expression)) {}

    Object visit (VisitorStmt &visitor) override {
        return visitor.acceptExpression(*this);
    }

    unique_ptr<Expr> expression;
};

class Print : public Stmt {
public:
    Print(unique_ptr<Expr> expression) : expression(std::move(expression)) {}

    Object visit (VisitorStmt &visitor) override {
        return visitor.acceptPrint(*this);
    }

    unique_ptr<Expr> expression;
};

class Var : public Stmt {
public:
    Var(unique_ptr<Token> name, unique_ptr<Expr> initializer) : name(std::move(name)), initializer(std::move(initializer)) {}

    Object visit (VisitorStmt &visitor) override {
        return visitor.acceptVar(*this);
    }

    unique_ptr<Token> name;
    unique_ptr<Expr> initializer;
};


#endif // STMT
