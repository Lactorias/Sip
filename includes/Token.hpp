

#include <string>
#include <strstream>
#include <variant>
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
  EOFF = EOF,
};


class Token {
public:
    using LiteralValue = std::variant<int, std::string>;
    Token(TokenType ty, std::string lexeme, LiteralValue literal, int line) : ty(ty), lexeme(lexeme), literal(literal), line(line) {}
    explicit operator std::string() const {
        return lexeme; 
    } 
    friend auto operator<<(std::ostream& os, const Token& token) -> std::ostream& {
        os << token.lexeme;
        return os;
    }
private:
    const TokenType ty;
    const std::string lexeme; // lex eeeeeeem --- not lex me
    const LiteralValue literal;
    const int line;
};
