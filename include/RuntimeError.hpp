#ifndef RUNTIMEERROR
#define RUNTIMEERROR

#include <Token.hpp>

class RuntimeError : public std::runtime_error {
public:
   explicit RuntimeError(Token& token, std::string message) : token(token), std::runtime_error(message) {} 

   auto runtime_error(RuntimeError error) -> void;
private:
    Token token;
};

#endif // RUNTIMEERROR