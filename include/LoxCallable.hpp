#ifndef LOXCALLABLE
#define LOXCALLABLE

#include <any>
#include <variant>
#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>
#include <SipVariant.hpp>
#include <iostream>


class Interpreter;
class Environment;
class Function;
struct Lox_Function;
struct Func;
struct Lox_Class;
struct Lox_Instance;
class Token;

using Lox_Callable = std::variant<std::monostate, Func, Lox_Function, Lox_Class, Lox_Instance>;

using Object = std::variant<std::monostate, int, std::string, double, bool, Lox_Callable>;



/*
    Functions users make in Lox.
*/

struct Lox_Function {

    Lox_Function(std::any declaration, std::shared_ptr<Environment> closure, bool is_initializer);

    auto arity() -> size_t;

    auto call(Interpreter &interpreter, std::vector<Object> &arguments) -> Object;

    auto bind(Lox_Instance instance) -> Lox_Function;

    bool is_initializer_m;
    std::any declaration_m;
    std::shared_ptr<Environment> closure_m;

};

/*
    Native functions.
*/

struct Func {

    Func(std::function<auto()->size_t> arity_t, std::function<auto(Interpreter &, std::vector<Object> &)->Object> call_t, std::string name_t); 
    
    std::string name;
    std::function<auto()->size_t> arity;
    std::function<auto(Interpreter &, std::vector<Object> &)->Object> call;

};


struct Lox_Class {


    Lox_Class(std::string name, std::any superclass, std::unordered_map<std::string, Lox_Function>& methods) : name(name)
        , methods(methods) 
        , superclass(superclass) {}

    operator std::string() {
        return name;
    }

    auto arity() -> size_t;

    auto call(Interpreter &interpreter, std::vector<Object> &arguments) -> Object;

    auto find_method(std::string name) -> Lox_Function;

    // of type Lox_Class 
    std::any superclass;
    std::string name;
    std::unordered_map<std::string, Lox_Function> methods;
};

struct Lox_Instance {

    Lox_Instance(Lox_Class klass) : klass(klass) {}

    auto get(Token& name) -> Object;

    auto set(Token& name, Object& value) -> void;

    operator std::string() {
        return klass.name + " instance";
    }
    
private:
    Lox_Class klass;
    std::unordered_map<std::string, Object> fields;
};

struct Arity {

    auto operator()(Lox_Callable& func) -> size_t {
        return std::visit(*this, func);
    }; 
    auto operator()(Func& func) -> size_t {
        return func.arity();
    }
    auto operator()(Lox_Function& func) -> size_t {
        return func.arity();
    }
    auto operator()(Lox_Class& func) -> size_t {
        return func.arity();
    }
    auto operator()(Lox_Instance& func) -> size_t {
        std::cout << "AHHHHHHHHHHHHHHHHHHHHHHHHHH" << '\n';
        return {};
    }
    auto operator()(std::monostate) -> size_t {
        return {};
    }

};

struct Callee {

    auto operator()(Lox_Callable& func) -> Object {
        return std::visit(*this, func);
    }; 
    auto operator()(Func& func) -> Object {
        return func.call(interpreter, args);
    }
    auto operator()(Lox_Function& func) -> Object {
        return func.call(interpreter, args);
    }
    auto operator()(Lox_Class& func) -> Object {
        return func.call(interpreter, args);
    }
    auto operator()(Lox_Instance& func) -> Object {
        std::cout << "AHHHHHHHHHHHHHHHHHHHHHHHHHH" << '\n';
        return {};
    }
    auto operator()(std::monostate) -> Object {
        return {};
    }

    Interpreter& interpreter;
    std::vector<Object>& args;

};





#endif // LOXCALLABLE