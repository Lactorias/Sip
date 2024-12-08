#include <Scanner.hpp>

auto Scanner::get_errlog() -> ErrLog const & { return errlog_; }

auto Scanner::add_token(Token::TokenType ty, Token::LiteralValue literal) -> void {
    auto text = source_.substr(start_, current_ - start_);
    std::cout << text << ": <------- lexeme is" << '\n'; 
    tokens_.push_back(Token(ty, std::move(text), literal, line_));
    start_ = current_;
}

auto Scanner::match(char expected) noexcept -> bool{
     if (at_end()) return false;
     if (source_[current_] != expected) return false;
     current_++;
     return true;
}

auto Scanner::peek() const noexcept -> char {
     if (at_end())
          return '\0';
     return source_[current_];
}

auto Scanner::peek_next() const noexcept -> char {
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
          errlog_.report(line_, "Unterminated String");
          return;
     }
     advance();

     const auto value = "\"" + source_.substr(start_ + 1, current_- 1);
     add_token(Token::TokenType::STRING, value);
}

auto Scanner::parse_number() -> void {
     while (is_digit(peek()))
          advance();
     if (peek() == '.' && is_digit(peek_next())) {
          advance();
          while (is_digit(peek()))
               advance();
     }
     add_token(Token::TokenType::NUMBER,
               std::stod(source_.substr(start_, current_- start_)));
}
auto Scanner::parse_identifier() -> void {
     while (isAlphaNumeric(peek()))
          advance();
     const auto text = source_.substr(start_, current_- start_);
     const auto it = keywords.find(text);
     const auto type =
          (it != keywords.end()) ? it->second : Token::TokenType::IDENTIFIER;
     add_token(type);
}

auto Scanner::scan_token() -> void {
    char c = advance();
    switch (c) {
    case '(':
        add_token(Token::TokenType::LEFT_PAREN);
        break;
    case ')':
        add_token(Token::TokenType::RIGHT_PAREN);
        break;
    case '{':
        add_token(Token::TokenType::LEFT_BRACE);
        break;
    case '}':
        add_token(Token::TokenType::RIGHT_BRACE);
        break;
    case ',':
        add_token(Token::TokenType::COMMA);
        break;
    case '.':
        add_token(Token::TokenType::DOT);
        break;
    case '-':
        add_token(Token::TokenType::MINUS);
        break;
    case '+':
        add_token(Token::TokenType::PLUS);
        break;
    case ';':
        add_token(Token::TokenType::SEMICOLON);
        break;
    case '*':
        add_token(Token::TokenType::STAR);
        break;
    case '!':
        add_token(match('=') ? Token::TokenType::BANG_EQUAL : Token::TokenType::BANG);
        break;
    case '=':
        add_token(match('=') ? Token::TokenType::EQUAL_EQUAL : Token::TokenType::EQUAL);
        break;
    case '<':
        add_token(match('=') ? Token::TokenType::LESS_EQUAL : Token::TokenType::LESS);
        break;
    case '>':
        add_token(match('=') ? Token::TokenType::GREATER_EQUAL : Token::TokenType::GREATER);
        break;
    case '/':
        if (match('/')) {
            while (peek() != '\n' && !at_end())
                advance();
        } else {
            add_token(Token::TokenType::SLASH);
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
        if (is_digit(c)) {
            parse_number();
        } else if (isalnum(c)) {
            parse_identifier();
        } else {
            errlog_.report(line_, "Unexpected character.");
        }
        break;
    }
}

auto Scanner::scan_tokens() -> std::vector<Token>  {
     while (not at_end()) {
          start_ = current_;
          scan_token();
     }
     Token::TokenType to_use = Token::TokenType::EOFF;
     Token::LiteralValue temp;
     Token eof = Token(to_use, "", temp, line_);
     tokens_.push_back(eof);
     return tokens_;
}