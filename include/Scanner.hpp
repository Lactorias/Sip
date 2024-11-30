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

#include <Token.hpp>
#include <ErrLog.hpp>

class Scanner {

public:
    Scanner(std::string source) : source_(source), errlog_{} {}

    auto scan_tokens() -> std::vector<Token>;
    auto get_errlog() -> ErrLog const&;

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

    auto add_token(Token::TokenType ty, Token::LiteralValue literal = {}) -> void;

private:
    inline static const std::unordered_map<std::string, Token::TokenType> keywords = {
        {"and", Token::TokenType::AND},       {"class", Token::TokenType::CLASS},
        {"else", Token::TokenType::ELSE},     {"false", Token::TokenType::FALSE},
        {"for", Token::TokenType::FOR},       {"fun",   Token::TokenType::FUN},
        {"if", Token::TokenType::IF},         {"nil",   Token::TokenType::NIL},
        {"or", Token::TokenType::OR},         {"print", Token::TokenType::PRINT},
        {"return", Token::TokenType::RETURN}, {"super", Token::TokenType::SUPER},
        {"this", Token::TokenType::THIS},     {"true",  Token::TokenType::TRUE},
        {"var", Token::TokenType::VAR},       {"while", Token::TokenType::WHILE},
    };

private:
    std::string const source_;
    std::vector<Token> tokens_ = {};
    size_t start_ = 0;
    size_t current_ = 0;
    size_t line_ = 1;
    ErrLog errlog_;
};

inline auto is_digit(char c) -> bool { return c >= '0' && c <= '9'; }
inline auto is_alpha(char c) -> char {
     return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
};
inline auto isAlphaNumeric(char c) -> bool { return is_alpha(c) || is_digit(c); }

#endif // SCANNER