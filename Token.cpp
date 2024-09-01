#include <stdio.h>
#include <string>
#include <variant>
#include "./includes/Token.hpp"

template <typename T>
class Object : public BaseObject {
public:
    Object(T value) : value_(value) {}
    T getValue() const {
        return value_;
    }
private:
    T value_;
};

using thing = std::variant<int, std::string>;

class Token::Token {
    const TokenType ty;
    const std::string lexeme; // lex eeeeeeem --- not lex me
    const thing literal;
    const int line;
public:
    std::string toString(){
        //return ty + " " + lexeme + " " + literal;
        return "h";
    }
};
