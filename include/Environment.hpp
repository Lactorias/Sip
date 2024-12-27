#ifndef ENVIRONMENT
#define ENVIRONMENT

#include <LoxCallable.hpp>
#include <Expr.hpp>
#include <unordered_map>
#include <RuntimeError.hpp>

class Environment {
public:
    std::shared_ptr<Environment> enclosing;

    Environment() { enclosing = nullptr; }

    Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing) {}

    auto define(std::string name, const Object value) -> void {
        values[name] = value;
    }

    auto get_at(int distance, const std::string& name) -> Object {
        return ancestor(distance).values[name];
        // if (distance <= 0 && values.contains(name)) {
        //     return values[name];
        // }
        // if (distance > 0 && enclosing) {
        //     return enclosing->get_at(distance - 1, name);
        // }
        // return 9999999;
    }

    auto assign_at(int distance, Token &name, Object &value) -> void {
        ancestor(distance).values[name.lexeme] = value;
        // if (distance <= 0 && values.contains(name.lexeme)) {
        //     values[name.lexeme] = value;
        //     return;
        // }
        // if (distance > 0 && enclosing) {
        //     enclosing->assign_at(distance - 1, name, value);
        //     return;
        // }
    }

    auto ancestor(int distance) -> Environment {
        auto environment = *this;
        for (int i = 0; i < distance; ++i) {
            environment = environment.enclosing;
        }
        return environment;
    }

    auto get(Token& name) -> Object {
        if (values.find(name.lexeme) != values.end()) {
            return values[name.lexeme];
        }
        if (enclosing != nullptr) return enclosing->get(name);
        throw RuntimeError(name, "Undefined variable got '" + name.lexeme + "'.");
    } 

    auto assign(Token& name, Object value) -> void {
        if (values.find(name.lexeme) != values.end()) {
            values[name.lexeme] = value;
            return;
        }
        if (enclosing != nullptr) {
            enclosing->assign(name, value);
            return;
        }
        throw RuntimeError(name, "Undefined variable assigned '" + name.lexeme + "'.");
    }

private:
    std::unordered_map<std::string, Object> values;
};


#endif // ENVIRONMENT