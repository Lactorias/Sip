#include <cctype>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
//#include "./includes/Token.hpp"
#include "Token.hpp"
#include "Sip.hpp"
class Scanner {
public:
    const std::string source;
    std::vector<Token> tokens;
    bool eaten_identifier = false;
    inline const static std::unordered_map<std::string, TokenType> keywords = {
        {"and", TokenType::AND},
        {"class", TokenType::CLASS},
        {"else", TokenType::ELSE},
        {"false", TokenType::FALSE},
        {"for", TokenType::FOR},
        {"fun", TokenType::FUN},
        {"if", TokenType::IF},
        {"nil", TokenType::NIL},
        {"or", TokenType::OR},
        {"print", TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"super", TokenType::SUPER},
        {"this", TokenType::THIS},
        {"true", TokenType::TRUE},
        {"var", TokenType::VAR},
        {"while", TokenType::WHILE},
    };
    Sip instance;

    size_t start = 0, current = 0, line = 1;

    Scanner(std::string source) : source(source) {}
   
    auto isAtEnd() const -> bool {
        return current >= source.length();
    } 

    auto advance() -> char {
        return source[current++];
    }

    auto addToken(TokenType ty, Token::LiteralValue literal = {}) -> void {
        std::string text = source.substr(start, current - start);
        tokens.push_back(Token(ty, std::move(text), literal, line));
        start = current;
    }

    auto match(char expected) -> bool {
        if (isAtEnd()) return false;
        if (source[current] != expected) return false;
        current++;
        return true;
    }

    char peek(){
        if (isAtEnd()) return '\0';
        return source[current];
    }

    auto peekNext() const -> char {
        if (current + 1 >= source.length()) return '\0';
        return source[current + 1];
    }

    auto isDigit(char c) const -> bool {
        return c >= '0' && c <= '9';
    }
    auto isAlpha(char c) -> char {
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'Z') ||
                c == '_';
    }

    auto isAlphaNumeric(char c) -> bool {
        return isAlpha(c) || isDigit(c);
    }

    void string(){
        while (peek() != '"' && !isAtEnd()){
            if (peek() == '\n') line++;
            advance();
        }
        if (isAtEnd()){
            instance.error(line, "Unterminated String");
            return;
        }
        advance();

        const auto value = source.substr(start + 1, current - 1);
        addToken(TokenType::STRING, value);
    }

    void number(){
        while (isDigit(peek())) advance();
        if (peek() == '.' && isDigit(peekNext())) {
            advance();
            while (isDigit(peek())) advance();
        }
        addToken(TokenType::NUMBER, std::stod(source.substr(start, current - start)));
    }
    auto identifier() -> void {
        while (isAlphaNumeric(peek())) advance();
        const auto text = source.substr(start, current - start);
        const auto it = keywords.find(text);
        const auto type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;
        addToken(type);
    }
    void scanToken(){
        char c = advance();
        switch (c) {
            case '(': addToken(TokenType::LEFT_PAREN); break;
            case ')': addToken(TokenType::RIGHT_PAREN); break;
            case '{': addToken(TokenType::LEFT_BRACE); break;
            case '}': addToken(TokenType::RIGHT_BRACE); break;
            case ',': addToken(TokenType::COMMA); break;
            case '.': addToken(TokenType::DOT); break;
            case '-': addToken(TokenType::MINUS); break;
            case '+': addToken(TokenType::PLUS); break;
            case ';': addToken(TokenType::SEMICOLON); break;
            case '*': addToken(TokenType::STAR); break;
            case '!': 
                addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
                break;
            case '=':
                addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
                break;
            case '<':
                addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
                break;
            case '>':
                addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
                break;
            case '/':
                if (match('/')){
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    addToken(TokenType::SLASH);
                }
                break;
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                line++;
                break;
            case '"':
                string();
                break;

            default:
                if (isDigit(c)){
                    number();
                } else if (isalnum(c)){
                    identifier();
                } else {
                    instance.error(line, "Unexpected character.");
                }
                break;
        }
    }


    std::vector<Token> scanTokens(){
        while(!isAtEnd()){
            start = current;
            scanToken();
        }
        TokenType to_use = TokenType::EOFF;
        Token::LiteralValue temp;
        Token eof = Token(to_use, "", temp, line);
        tokens.push_back(eof);
        return tokens;
    }
};
