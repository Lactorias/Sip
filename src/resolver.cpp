#include <Resolver.hpp>
#include <Parser.hpp>

auto Resolver::acceptExpr(Expr &expr) -> Object { return {}; }

auto Resolver::acceptStmt(Stmt &stmt) -> Object { return {}; } 

auto Resolver::acceptBlock(Block &block) -> Object {
    begin_scope();
    resolve(block.statements);
    end_scope();
    return {};
}

auto Resolver::acceptVar(Var &var) -> Object {
    declare(var.name);
    if (var.initializer != nullptr) {
        resolve(*var.initializer);
    }
    define(var.name);
    return {};
}

auto Resolver::acceptVariable(Variable &variable) -> Object {
    if (   !scopes.empty() 
         && scopes.back().contains(variable.name->lexeme) 
         && scopes.back()[variable.name->lexeme] == false) {

        sip_logger.error(*variable.name, "Can't read local variable within its own initializer.");
    }
    resolve_local(variable, *variable.name);
    return {};
}

auto Resolver::accept_Class(_Class &_class) -> Object {
    declare(_class.name);
    define(_class.name);
    if (_class.superclass != nullptr && _class.name->lexeme == _class.superclass->name->lexeme) {
        sip_logger.error(*_class.superclass->name, "A class can't inherit from itself.");
    }
    if (_class.superclass != nullptr) resolve(*_class.superclass);
    if (_class.superclass != nullptr) {
        begin_scope();
        scopes.back()["super"] = true;
    }
    begin_scope();
    scopes.back()["this"] = true;
    for (auto method : _class.methods) {
        auto decl = Function_Type::METHOD;
        if (method->name->lexeme == "init") {
            decl = Function_Type::INITIALIZER;
        }
        resolve_function(method, decl);
    }
    end_scope();
    if (_class.superclass != nullptr) end_scope();
    return {};
}

auto Resolver::acceptSuper(Super &super) -> Object {
    resolve_local(super, *super.keyword);
    return {};
}

auto Resolver::accept_This(_This &_this) -> Object {
    resolve_local(_this, *_this.keyword);
    return {};
}

auto Resolver::acceptGet(Get &get) -> Object {
    resolve(*get.object);
    return {};
}

auto Resolver::acceptSet(Set &set) -> Object {
    resolve(*set.value);
    resolve(*set.object);
    return {};
}

auto Resolver::acceptAssign(Assign &assign) -> Object {
    resolve(*assign.value);
    resolve_local(assign, *assign.name);
    return {};
}

auto Resolver::acceptFunction(Function &function) -> Object {
    declare(function.name);
    define(function.name);
    resolve_function(std::make_shared<Function>(function), Function_Type::FUNCTION);
    return {};
}

auto Resolver::acceptExpression(Expression &expression) -> Object {
    resolve(*expression.expression);
    return {};
}

auto Resolver::accept_If(_If &_if) -> Object {
    resolve(*_if.condition);
    resolve(*_if.then_branch);
    if (_if.else_branch != nullptr) resolve(*_if.else_branch);
    return {};
}

auto Resolver::acceptPrint(Print &print) -> Object {
    resolve(*print.expression);
    return {};
}

auto Resolver::accept_Return(_Return &_return) -> Object {
    if (current_function == Function_Type::NONE) {
        sip_logger.error(*_return.keyword, "Can't return from top-level code.");
    }
    if (current_function == Function_Type::INITIALIZER) {
        sip_logger.error(*_return.keyword, "Can't return a value from an initializer.");
    }
    if (_return.value != nullptr) {
        resolve(*_return.value);
    }
    return {};
}

auto Resolver::accept_While(_While &_while) -> Object {
    resolve(*_while.condition);
    resolve(*_while.body);
    return {};
}

auto Resolver::acceptBinary(Binary &binary) -> Object {
    resolve(*binary.left);
    resolve(*binary.right);
    return {};
}

auto Resolver::acceptCall(Call &call) -> Object {
    resolve(*call.callee);
    for (auto argument : call.arguments) {
        resolve(*argument);
    }
    return {};
}

auto Resolver::acceptGrouping(Grouping &grouping) -> Object {
    resolve(*grouping.expression);
    return {};
}

auto Resolver::acceptLiteral(const Literal &literal) -> Object {
    return {};
}

auto Resolver::acceptLogical(Logical &logical) -> Object {
    resolve(*logical.left);
    resolve(*logical.right);
    return {};
}

auto Resolver::acceptUnary(Unary &unary) -> Object {
    resolve(*unary.right);
    return {};
}

auto Resolver::resolve_function(std::shared_ptr<Function> function, Function_Type type) -> void {
    auto enclosing_function = current_function;
    current_function = type;
    begin_scope();
    for (auto param : function->params) {
        declare(param);
        define(param);
    }
    resolve(function->body);
    end_scope();
    current_function = enclosing_function;
}

auto Resolver::resolve_local(Expr &expr, Token &name) -> void {
    for (int i = scopes.size() - 1; i >= 0; --i) {
        if (scopes[i].contains(name.lexeme)) {
            interpreter->resolve(expr, static_cast<int>(scopes.size() - 1 - i));
            return;
        }
    }
}

auto Resolver::declare(shared_ptr<Token> name) -> void {
    if (scopes.empty()) return;
    auto scope = scopes.back();
    if (scope.contains(name->lexeme)) {
        sip_logger.error(*name, "Already a variable with this name in scope.");
    }
    scope[name->lexeme] = false;
}

auto Resolver::define(shared_ptr<Token> name) -> void {
    if (scopes.empty()) return;
    scopes.back()[name->lexeme] = true;
}

auto Resolver::resolve(std::vector<std::shared_ptr<Stmt>>& statements) -> void {
    for (auto statement : statements) {
        resolve(*statement);
    }
}

auto Resolver::resolve(Stmt& stmt) -> void {
    stmt.visit(*this);
}

auto Resolver::resolve(Expr& expr) -> void {
    expr.visit(*this);
}

auto Resolver::begin_scope() -> void {
    scopes.emplace_back();
}

auto Resolver::end_scope() -> void {
    scopes.pop_back();   
}