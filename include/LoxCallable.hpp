#ifndef LOXCALLABLE
#define LOXCALLABLE

#include <any>
#include <variant>
#include <functional>
#include <memory>
#include <vector>


class Interpreter;
class Environment;
class Function;
struct Lox_Function;
struct Func;

using Lox_Callable = std::variant<std::monostate, Func, Lox_Function>;

using Object = std::variant<std::monostate, int, std::string, double, bool, Lox_Callable>;



/*
    Functions users make in Lox.
*/

struct Lox_Function {

    Lox_Function(std::any declaration, std::shared_ptr<Environment> closure);

    auto arity() -> size_t;

    auto call(Interpreter &interpreter, std::vector<Object> &arguments) -> Object;

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
    auto operator()(std::monostate) -> Object {
        return {};
    }

    Interpreter& interpreter;
    std::vector<Object>& args;

};





#endif // LOXCALLABLE