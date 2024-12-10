#ifndef STMT
#define STMT
#include <Token.hpp>
#include <Expr.hpp>
#include <variant>
#include <memory>
#include <utility>
using Object = std::variant<std::monostate, int, std::string, double, bool>;
using std::unique_ptr;

class Expression;
class Print;
class Stmt;


class Visitor {
public:
    virtual Object acceptExpression(Expression &expression) = 0;

    virtual Object acceptPrint(Print &print) = 0;

    virtual Object acceptStmt(Stmt &stmt) = 0;

};
class Stmt {
public:
    virtual Object visit(Visitor &visitor) = 0;
};

class Expression : public Stmt {
public:
    Expression(unique_ptr<Expr> expression) : expression(std::move(expression)) {}

    Object visit(Visitor &visitor) override {
        return visitor.acceptExpression(*this);
    }

    unique_ptr<Expr> expression;
};

class Print : public Stmt {
public:
    Print(unique_ptr<Expr> expression) : expression(std::move(expression)) {}

    Object visit(Visitor &visitor) override {
        return visitor.acceptPrint(*this);
    }

    unique_ptr<Expr> expression;
};


#endif // STMT
