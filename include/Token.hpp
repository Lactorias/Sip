

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

inline auto token_string(TokenType ty) -> std::string {
    switch(ty){
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::COMMA: return "COMMA";
        case TokenType::DOT: return "DOT";
        case TokenType::MINUS: return "MINUS";
        case TokenType::PLUS: return "PLUS";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::SLASH: return "SLASH";
        case TokenType::STAR: return "STAR";
        case TokenType::BANG: return "BANG";
        case TokenType::BANG_EQUAL: return "BANG_EQUAL";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING: return "STRING";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::AND: return "AND";
        case TokenType::CLASS: return "CLASS";
        case TokenType::ELSE: return "ELSE";
        case TokenType::FALSE: return "FALSE";
        case TokenType::FUN: return "FUN";
        case TokenType::FOR: return "FOR";
        case TokenType::IF: return "IF";
        case TokenType::NIL: return "NIL";
        case TokenType::OR: return "OR";
        case TokenType::PRINT: return "PRINT";
        case TokenType::RETURN: return "RETURN";
        case TokenType::SUPER: return "SUPER";
        case TokenType::THIS: return "THIS";
        case TokenType::TRUE: return "TRUE";
        case TokenType::VAR: return "VAR";
        case TokenType::WHILE: return "WHILE";
        case TokenType::INT: return "INT";
        case TokenType::BOOL: return "BOOL";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::CHAR: return "CHAR";
        case TokenType::DOUBLE: return "DOUBLE";
        case TokenType::EOFF: return "EOFF";
    }
}

    inline auto operator<<(std::ostream& os, const TokenType& token) -> std::ostream& {
        os << token_string(token);
        return os;
    }

class Token {
public:
    using LiteralValue = std::variant<std::monostate, int, std::string, double>;
    Token(TokenType ty, std::string lexeme, LiteralValue literal, size_t line) : ty(ty), lexeme(lexeme), literal(literal), line(line) {}
    explicit operator std::string() const {
        return lexeme; 
    } 
    friend auto operator<<(std::ostream& os, const Token& token) -> std::ostream& {
        os << token.lexeme;
        return os;
    }
    const TokenType ty;
    const std::string lexeme; // lex eeeeeeem --- not lex me
    const LiteralValue literal;
    const size_t line;
};
