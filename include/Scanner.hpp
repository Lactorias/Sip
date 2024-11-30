#ifndef SCANNER
#define SCANNER

#include <cctype>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <Sip.hpp>
#include <Token.hpp>

class Scanner {

public:
    Scanner(std::string source) : source_(source) {}

    auto scan_tokens() -> std::vector<Token>;

private:
    inline auto at_end() const noexcept -> bool { return current_ >= source_.length(); }
    inline auto advance() -> char { return source_[current_++]; }

    auto peek() const noexcept -> char;
    auto peek_next() const noexcept -> char;
    auto match(char expected) noexcept -> bool;

    auto parse_string() -> void ;
    auto parse_number() -> void ;
    auto parse_identifier() -> void;

    auto scan_token() -> void;

    auto add_token(TokenType ty, Token::LiteralValue literal = {}) -> void;

private:
    inline static const std::unordered_map<std::string, TokenType> keywords = {
        {"and", TokenType::AND},       {"class", TokenType::CLASS},
        {"else", TokenType::ELSE},     {"false", TokenType::FALSE},
        {"for", TokenType::FOR},       {"fun", TokenType::FUN},
        {"if", TokenType::IF},         {"nil", TokenType::NIL},
        {"or", TokenType::OR},         {"print", TokenType::PRINT},
        {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
        {"this", TokenType::THIS},     {"true", TokenType::TRUE},
        {"var", TokenType::VAR},       {"while", TokenType::WHILE},
    };

private:
    std::string const source_;
    std::vector<Token> tokens_;
    size_t start_ = 0;
    size_t current_ = 0;
    size_t line_ = 1;
    Sip instance_;
};

inline auto isDigit(char c) -> bool { return c >= '0' && c <= '9'; }
inline auto isDigit(char c) -> bool { return c >= '0' && c <= '9'; }
inline auto isAlpha(char c) -> char {
     return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
};
inline auto isAlphaNumeric(char c) -> bool { return isAlpha(c) || isDigit(c); }

#endif // SCANNER