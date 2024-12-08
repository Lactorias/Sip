#include <ErrLog.hpp>
#include <Token.hpp>

auto ErrLog::report(size_t line, std::string error) -> void {
    logs_.emplace_back(line, std::move(error));
}

auto ErrLog::error(Token token, std::string message) noexcept -> void {
    if (token.ty == Token::TokenType::EOFF) report(token.line, " at end" + message);
    else report(token.line, " at '" + token.lexeme + "'" + message);
}

auto ErrLog::empty() const noexcept -> bool { return logs_.empty(); }

// err << os

// l, r
// 

// inside L
// l.<<(r)
// l << r
