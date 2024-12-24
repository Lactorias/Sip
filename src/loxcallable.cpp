#include <LoxCallable.hpp>
#include <Environment.hpp>
#include <Interpreter.hpp>
#include <Return.hpp>

Lox_Function::Lox_Function(std::any declaration, std::shared_ptr<Environment> closure)
    : declaration_m(declaration)
    , closure_m(closure) {}

auto Lox_Function::arity() -> size_t { return std::any_cast<Function>(declaration_m).params.size(); }

auto Lox_Function::call(Interpreter &interpreter, std::vector<Object> &arguments) -> Object { 
    auto environment = std::make_shared<Environment>(closure_m);
    for (int i = 0; i < std::any_cast<Function>(declaration_m).params.size(); ++i) {
        environment->define(std::any_cast<Function>(declaration_m).params[i]->lexeme, arguments[i]);
    }
    try {
        interpreter.execute_block(std::any_cast<Function>(declaration_m).body, environment);
    } catch (Object& return_value) {
        return return_value; 
    }
    return {};
}

Func::Func(std::function<auto()->size_t> arity_t, std::function<auto(Interpreter &, std::vector<Object> &)->Object> call_t, std::string name_t)
    : name(name_t)
    , call(call_t)
    , arity(arity_t) {} 