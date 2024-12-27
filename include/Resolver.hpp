#ifndef RESOLVER
#define RESOLVER

#include <Interpreter.hpp>
#include <ErrLog.hpp>
#include <stack>
#include <unordered_map>

class Resolver : public VisitorExpr, public VisitorStmt {
public:

    Resolver(std::shared_ptr<Interpreter> interpreter) : interpreter(interpreter) {}

    virtual Object accept_While(_While &_while) override;

    virtual Object acceptFunction(Function &function) override;

    virtual Object accept_Return(_Return &_return) override;

    virtual Object acceptCall(Call &call) override;

    virtual Object accept_If(_If &_if) override;

    virtual Object acceptLogical(Logical &logical) override;

    virtual Object acceptBlock(Block &block) override; 

    virtual Object acceptAssign(Assign &assign) override;

    virtual Object acceptVariable(Variable &variable) override;

    virtual Object acceptVar(Var &var) override;

    virtual Object accept_Class(_Class &_class) override;

    virtual Object acceptGet(Get &get) override;

    virtual Object accept_This(_This &_this) override;

    virtual Object acceptSet(Set &set) override;

    virtual Object acceptExpression(Expression &expression) override;

    virtual Object acceptSuper(Super &super) override;

    virtual Object acceptPrint(Print &print) override;

    virtual Object acceptStmt(Stmt &stmt) override;

    virtual Object acceptExpr(Expr &expr) override;

    virtual Object acceptBinary(Binary &binary) override;

    virtual Object acceptGrouping(Grouping &grouping) override;

    virtual Object acceptLiteral(const Literal &literal) override;

    virtual Object acceptUnary(Unary &unary) override;

    auto resolve(std::vector<std::shared_ptr<Stmt>>& statements) -> void;

private:

    enum class Function_Type {
        NONE,
        FUNCTION,
        INITIALIZER,
        METHOD,
    };

private:

    auto declare(shared_ptr<Token> name) -> void;

    auto define(shared_ptr<Token> name) -> void;

    auto resolve(Stmt& stmt) -> void;

    auto resolve(Expr& expr) -> void;

    auto resolve_local(Expr &expr, Token &name) -> void;

    auto resolve_function(std::shared_ptr<Function> function, Function_Type type) -> void;

    auto begin_scope() -> void;

    auto end_scope() -> void;

private:

    Function_Type current_function = Function_Type::NONE; 
    std::shared_ptr<Interpreter> interpreter;
    std::vector<std::unordered_map<std::string, bool>> scopes;

public:

    ErrLog sip_logger;

};


#endif // RESOLVER