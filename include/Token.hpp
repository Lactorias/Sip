#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <strstream>
#include <variant>


struct Token {
    // Forward decl
    enum class TokenType;

public:
    using LiteralValue = std::variant<std::monostate, int, std::string, double, bool>;

    Token(TokenType ty, std::string lexeme, LiteralValue literal, size_t line) 
        : ty(ty), lexeme(lexeme), literal(literal), line(line) {}

    explicit operator std::string() const { return lexeme; } 

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

        EOFF = EOF, 
    };

public:
    const TokenType ty;
    const std::string lexeme; // lex eeeeeeem --- not lex me
    const LiteralValue literal;
    const size_t line;
};

auto token_string(Token::TokenType ty) -> char const *;

inline auto operator<<(std::ostream &os, Token::TokenType const& token) -> std::ostream & {
    return os << token_string(token);
}

inline auto operator<<(std::ostream& os, Token const& token) -> std::ostream& {
    return os << token.lexeme;
}

#endif // TOKEN_HPP