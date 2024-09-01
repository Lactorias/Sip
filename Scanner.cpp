#include <stdio.h>
#include <string>
#include <vector>
#include "./includes/Token.hpp"
#include "./includes/Sip.hpp"

class Scanner {
private:
    const std::string source;
    std::vector<Token> tokens;
    Sip instance;

    int start = 0, current = 0, line = 1;

    Scanner(std::string source) : source(source) {}
   
    auto isAtEnd() const -> bool {
        return current >= source.length();
    } 

    char advance(){
        return source[current++];
    }

    void addToken(TokenType ty, Token::LiteralValue literal = {}){
        std::string text = source.substr(start, current);
        tokens.push_back(Token(ty, text, literal, line));
    }

    bool match(char expected){
        if (isAtEnd()) return false;
        if (source[current] != expected) return false;
        current++;
        return true;
    }

    char peek(){
        if (!isAtEnd()) return '\0';
        return source[current];
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
                instance.error(line, "Unexpected character.");
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
