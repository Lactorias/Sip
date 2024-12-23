#include <LoxCallable.hpp>
#include <Environment.hpp>
#include <Interpreter.hpp>

Lox_Function::Lox_Function(std::any declaration)
    : declaration_m(declaration) {}

auto Lox_Function::arity() -> size_t { return std::any_cast<Function>(declaration_m).params.size(); }

auto Lox_Function::call(Interpreter &interpreter, std::vector<Object> &arguments) -> Object { 
    auto environment = std::make_shared<Environment>(interpreter.globals);
    for (int i = 0; i < std::any_cast<Function>(declaration_m).params.size(); ++i) {
        environment->define(std::any_cast<Function>(declaration_m).params[i]->lexeme, arguments[i]);
    }
    interpreter.execute_block(std::any_cast<Function>(declaration_m).body, environment);
    return {};
}

Func::Func(std::function<auto()->size_t> arity_t, std::function<auto(Interpreter &, std::vector<Object> &)->Object> call_t, std::string name_t)
    : name(name_t)
    , call(call_t)
    , arity(arity_t) {} 