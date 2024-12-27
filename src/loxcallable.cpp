#include <LoxCallable.hpp>
#include <Environment.hpp>
#include <Interpreter.hpp>
#include <Return.hpp>

Lox_Function::Lox_Function(std::any declaration, std::shared_ptr<Environment> closure, bool is_initializer)
    : declaration_m(declaration)
    , closure_m(closure) 
    , is_initializer_m(is_initializer) {}

auto Lox_Function::arity() -> size_t { return std::any_cast<Function>(declaration_m).params.size(); }

auto Lox_Function::call(Interpreter &interpreter, std::vector<Object> &arguments) -> Object { 
    auto environment = std::make_shared<Environment>(closure_m);
    for (int i = 0; i < std::any_cast<Function>(declaration_m).params.size(); ++i) {
        environment->define(std::any_cast<Function>(declaration_m).params[i]->lexeme, arguments[i]);
    }
    try {
        interpreter.execute_block(std::any_cast<Function>(declaration_m).body, environment);
    } catch (Object& return_value) {
        if (is_initializer_m) return closure_m->get_at(0, "this");
        return return_value; 
    }
    if (is_initializer_m) return closure_m->get_at(0, "this");
    return {};
}

auto Lox_Function::bind(Lox_Instance instance) -> Lox_Function {
    auto environment = std::make_shared<Environment>(closure_m);
    environment->define("this", static_cast<Lox_Callable>(instance));
    return Lox_Function(declaration_m, environment, is_initializer_m);
}

auto Lox_Class::call(Interpreter &interpreter, std::vector<Object> &arguments) -> Object {
    auto instance = std::make_shared<Lox_Instance>(*this);
    auto initializer = find_method("init");
    if (initializer.closure_m != nullptr) {
        initializer.bind(*instance).call(interpreter, arguments);
    }
    return static_cast<Lox_Callable>(*instance);
}

auto Lox_Class::arity() -> size_t { 
    auto initializer = find_method("init");
    if (initializer.closure_m == nullptr) return 0;
    return initializer.arity();    
}

auto Lox_Class::find_method(std::string name) -> Lox_Function {
        if (methods.contains(name)) {
            return methods.at(name);
        }
        return Lox_Function("", nullptr, false);
    }

Func::Func(std::function<auto()->size_t> arity_t, std::function<auto(Interpreter &, std::vector<Object> &)->Object> call_t, std::string name_t)
    : name(name_t)
    , call(call_t)
    , arity(arity_t) {} 



auto Lox_Instance::get(Token& name) -> Object {
        if (fields.contains(name.lexeme)) {
            return fields[name.lexeme];
        }
        auto method = klass.find_method(name.lexeme);
        if (method.closure_m != nullptr) return method.bind(*this);
        throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
    }

auto Lox_Instance::set(Token& name, Object& value) -> void {
    fields[name.lexeme] = value;
}