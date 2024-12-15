#ifndef INTERPRETER
#define INTERPRETER

#include <Expr.hpp>
#include <Token.hpp>
#include <Stmt.hpp>
#include <vector>
#include <Environment.hpp>

/*
    To interpret our language in the early stages, we will directly execute the syntax tree itself.
*/

class Interpreter : public VisitorExpr, public VisitorStmt {
public:

    auto interpret(std::vector<std::shared_ptr<Stmt>> statements) -> void;

    virtual Object accept_While(_While &_while) override;

    virtual Object accept_If(_If &_if) override;

    virtual Object acceptLogical(Logical &logical) override;

    virtual Object acceptBlock(Block &block) override; 

    virtual Object acceptAssign(Assign &assign) override;

    virtual Object acceptVariable(Variable &variable) override;

    virtual Object acceptVar(Var &var) override;

    virtual Object acceptExpression(Expression &expression) override;

    virtual Object acceptPrint(Print &print) override;

    virtual Object acceptStmt(Stmt &stmt) override;

    virtual Object acceptExpr(Expr &expr) override;

    virtual Object acceptBinary(Binary &binary) override;

    virtual Object acceptGrouping(Grouping &grouping) override;

    virtual Object acceptLiteral(const Literal &literal) override;

    virtual Object acceptUnary(Unary &unary) override;

private:
    auto evaluate(Expr &expr) -> Object;

    auto execute(std::shared_ptr<Stmt> stmt) -> void;

    auto is_truth(const Object &object) -> bool;

    auto is_equal(Object &a, Object &b) -> bool;

    auto extract_double(const Object& object) -> double;

    auto stringify(Object object) -> std::string;

    auto execute_block(std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> environment) -> void;

    auto check_number_operand(Token& op, Object &operand) -> void;

    auto check_number_operand(Token& op, Object& left, Object& right) -> void;

private:
    std::shared_ptr<Environment> environment = std::make_shared<Environment>();
};

#endif // INTERPRETER
