#include "../include/Scanner.hpp"

auto Scanner::add_token(TokenType ty, Token::LiteralValue literal = {}) -> void {
    auto text = source_.substr(start_, current_ - start_);
    tokens_.push_back(Token(ty, std::move(text), literal, line_));
    start_ = current_;
}

auto Scanner::match(char expected) -> bool{
     if (at_end()) return false;
     if (source_[current_] != expected) return false;
     current_++;
     return true;
}

auto Scanner::peek() const -> char {
     if (at_end())
          return '\0';
     return source_[current_];
}

auto Scanner::peek_next() const -> char {
     if (current_+ 1 >= source_.length())
          return '\0';
     return source_[current_+ 1];
}

auto Scanner::parse_string() -> void {
     while (peek() != '"' && !at_end()) {
          if (peek() == '\n')
               line_++;
          advance();
     }
     if (at_end()) {
          instance_.error(line_, "Unterminated String");
          return;
     }
     advance();

     const auto value = source_.substr(start_ + 1, current_- 1);
     add_token(TokenType::STRING, value);
}

auto Scanner::parse_number() -> void {
     while (isDigit(peek()))
          advance();
     if (peek() == '.' && isDigit(peek_next())) {
          advance();
          while (isDigit(peek()))
               advance();
     }
     add_token(TokenType::NUMBER,
               std::stod(source_.substr(start_, current_- start_)));
}
auto Scanner::parse_identifier() -> void {
     while (isAlphaNumeric(peek()))
          advance();
     const auto text = source_.substr(start_, current_- start_);
     const auto it = keywords.find(text);
     const auto type =
          (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;
     add_token(type);
}

auto Scanner::scan_token() -> void {
    char c = advance();
    switch (c) {
    case '(':
        add_token(TokenType::LEFT_PAREN);
        break;
    case ')':
        add_token(TokenType::RIGHT_PAREN);
        break;
    case '{':
        add_token(TokenType::LEFT_BRACE);
        break;
    case '}':
        add_token(TokenType::RIGHT_BRACE);
        break;
    case ',':
        add_token(TokenType::COMMA);
        break;
    case '.':
        add_token(TokenType::DOT);
        break;
    case '-':
        add_token(TokenType::MINUS);
        break;
    case '+':
        add_token(TokenType::PLUS);
        break;
    case ';':
        add_token(TokenType::SEMICOLON);
        break;
    case '*':
        add_token(TokenType::STAR);
        break;
    case '!':
        add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        break;
    case '=':
        add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        break;
    case '<':
        add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        break;
    case '>':
        add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;
    case '/':
        if (match('/')) {
            while (peek() != '\n' && !at_end())
                advance();
        } else {
            add_token(TokenType::SLASH);
        }
        break;
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        line_++;
        break;
    case '"':
        parse_string();
        break;

    default:
        if (isDigit(c)) {
            parse_number();
        } else if (isalnum(c)) {
            parse_identifier();
        } else {
            instance_.error(line_, "Unexpected character.");
        }
        break;
    }
}

auto Scanner::scan_tokens() -> std::vector<Token>  {
     while (not at_end()) {
          start_ = current_;
          scan_token();
     }
     TokenType to_use = TokenType::EOFF;
     Token::LiteralValue temp;
     Token eof = Token(to_use, "", temp, line_);
     tokens_.push_back(eof);
     return tokens_;
}