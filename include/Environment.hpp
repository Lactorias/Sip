#ifndef ENVIRONMENT
#define ENVIRONMENT

#include <Expr.hpp>
#include <unordered_map>
#include <RuntimeError.hpp>

class Environment {
public:

    auto define(std::string name, Object value) -> void {
        values[name] = value;
    }

    auto get(Token& name) -> Object {
        if (values.find(name.lexeme) != values.end()) {
            return values[name.lexeme];
        }
        throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
    } 
private:
    std::unordered_map<std::string, Object> values;
};


#endif // ENVIRONMENT