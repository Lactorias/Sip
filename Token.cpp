#include <stdio.h>
#include <string>
#include "./includes/Token.hpp"


class BaseObject::BaseObject {
public:
    virtual ~BaseObject() = default;

    virtual std::string toString() const {
        return "BaseObject";
    }

    virtual bool equals(const BaseObject& other) const {
        return this == &other;
    }
};

class Token::Token {
    const TokenType ty;
    const std::string lexeme; // lex eeeeeeem --- not lex me
    const BaseObject literal;
    const int line;
    Token(TokenType ty, std::string lexeme, BaseObject literal, int line) : ty(ty), lexeme(lexeme), literal(literal), line(line) {}
public:
    std::string toString(){
        //return ty + " " + lexeme + " " + literal;
        return "h";
    }
};
