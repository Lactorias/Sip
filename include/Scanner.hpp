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

/*
    Sip is a regular language, such that its language is simple enough to be deduced through regular expressions. The following however is a more hands on approach to scanning!
    The following class "Scanner" is what produces our tokens to interpret from, it is essentially a loop with an exhaustive switch case, aimed to produce tokens on the current
    state of a lexeme. When the lexeme is exhausted/completed our token is finished and is allowed to be created and given to our Tokens vector! Neat!
*/
class Scanner {

public:

    /* 
        Scanner is constructed on-top of the source code passed here through main.cpp. We have logic to iterate through this source code and produce our tokens.
    */
    Scanner(std::string source) : source_(source), errlog_{} {}
    /*
        scan_tokens() makes this iteration through the source code possible, it begins our scanning process, meaning
        it is a public function because of this.
    */
    auto scan_tokens() -> std::vector<Token>;
    /*
        get_errlog(), obtains a const reference to the ErrLog object within the scanner class.
    */
    auto get_errlog() -> ErrLog const&;

private:
    /*
        at_end() determines if we still have characters in our source code to consume for our tokens.
    */
    inline auto at_end() const noexcept -> bool { return current_ >= source_.length(); }
    /*
        advance() moves our current index forwards through the source code.
    */
    inline auto advance() -> char { return source_[current_++]; }

    /*
    */
    auto peek() const noexcept -> char;
    /*
        peek_next(), there can be circumstances where we wish to view what the next character in the source code is,
        this is especially useful in cases where we want to consume an entire literal value, and advance while
        the next character is indeed a valid digit.
    */
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