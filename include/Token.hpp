#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <strstream>
#include <variant>
#include <LoxCallable.hpp>

/*
    Our Token struct, forms the building blocks of our language, they are sections of source code
    bundled up into neat packages which contain some key information! 
    TokenType ty: TokenType deduces what type of token, our token is, so we dont compare against a tokens lexeme value, instead we compare an enum token type value.
    Lexeme: The actual information of the token, so a lexeme could be "while", while the type itself is TokenType::WHILE
    Literal: A location to store the actual literal value a lexeme may have, for use later. E.g "hello", 1, 8.7 <- std::variant because of this!
    Line: Where the token itself occurs, used for error handling when needed.
*/
struct Token {
    /*
        Forward declaration, especially clean with "enum class" <- underlying type of, int.
    */
    enum class TokenType;

public:
    using LiteralValue = std::variant<std::monostate, int, std::string, double, bool, Lox_Callable>;

    Token(TokenType ty, std::string lexeme, LiteralValue literal, size_t line) 
        : ty(ty), lexeme(lexeme), literal(literal), line(line) {}

    //explicit operator std::string() const { return lexeme; } 

    /*
        Serves as the possible TokenTypes, and also essentially the reserved keywords for the Sip language.
    */
    enum class TokenType {
        // Single-character tokens.
        LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
        COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

        // One or two character tokens.
        BANG, BANG_EQUAL,
        EQUAL, EQUAL_EQUAL,
        GREATER, GREATER_EQUAL,
        LESS, LESS_EQUAL,

        // Literals, identifier = variable name.
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
    const std::string lexeme; 
    const LiteralValue literal;
    const size_t line;
};

auto token_string(Token::TokenType ty) -> char const *;

/*
    Operator overloads to easily print token types in whatever stream requires it!
*/
inline auto operator<<(std::ostream &os, Token::TokenType const& token) -> std::ostream & {
    return os << token_string(token);
}
/*
    Operator overloads to easily print token lexemes in whatever stream requires it!
*/
inline auto operator<<(std::ostream& os, Token const& token) -> std::ostream& {
    return os << token.lexeme;
}

#endif // TOKEN_HPP