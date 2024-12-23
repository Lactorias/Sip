#ifndef ENVIRONMENT
#define ENVIRONMENT

#include <SipVariant.hpp>
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

    auto get(Token& name) -> Object {
        if (values.find(name.lexeme) != values.end()) {
            return values[name.lexeme];
        }
        if (enclosing != nullptr) return enclosing->get(name);
        throw RuntimeError(name, "Undefined variable got '" + name.lexeme + "'.");
    } 

    auto assign(Token& name, Object& value) -> void {
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