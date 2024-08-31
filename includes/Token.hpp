#include <string>

enum class TokenType {
    // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  // Data Types
  INT, BOOL, FLOAT, CHAR, DOUBLE,

  //End of File
  EOFF
};

class BaseObject {
public:
    virtual ~BaseObject() = default;
    virtual std::string toString();
    virtual bool equals(const BaseObject& other);
};

class Token {
    const TokenType ty;
    const std::string lexeme; // lex eeeeeeem --- not lex me
    const BaseObject literal;
    const int line;
    Token(TokenType ty, std::string lexeme, BaseObject literal, int line);
public:
    std::string toString();
};
