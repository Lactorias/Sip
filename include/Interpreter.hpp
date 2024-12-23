#ifndef INTERPRETER
#define INTERPRETER

#include <Expr.hpp>
#include <Token.hpp>
#include <Stmt.hpp>
#include <vector>
#include <chrono>
#include <Environment.hpp>
#include <LoxCallable.hpp>

/*
    To interpret our language in the early stages, we will directly execute the syntax tree itself.
*/

class Interpreter : public VisitorExpr, public VisitorStmt {
public:

    Interpreter() {
        globals->define("clock", Func{[]() { return 0; },
                                      [](auto const & /* interpreter */, auto const & /* args */) {
                                        using namespace std::chrono;
                                        return static_cast<double>(
                                            duration_cast<seconds>(system_clock::now().time_since_epoch())
                                            .count());
                                      }, "clock"}); 
    }

    auto execute_block(const std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> environment) -> void;

    auto interpret(std::vector<std::shared_ptr<Stmt>> statements) -> void;

    virtual Object accept_While(_While &_while) override;

    virtual Object acceptFunction(Function &function) override;

    virtual Object acceptCall(Call &call) override;

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

    auto check_number_operand(Token& op, Object &operand) -> void;

    auto check_number_operand(Token& op, Object& left, Object& right) -> void;

public:
    std::shared_ptr<Environment> globals = std::make_shared<Environment>();
    std::shared_ptr<Environment> environment = globals;
};

#endif // INTERPRETER
