#include <Token.hpp>

auto token_string(Token::TokenType ty) -> char const* {
    switch (ty) {
        case Token::TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case Token::TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case Token::TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case Token::TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case Token::TokenType::COMMA: return "COMMA";
        case Token::TokenType::DOT: return "DOT";
        case Token::TokenType::MINUS: return "MINUS";
        case Token::TokenType::PLUS: return "PLUS";
        case Token::TokenType::SEMICOLON: return "SEMICOLON";
        case Token::TokenType::SLASH: return "SLASH";
        case Token::TokenType::STAR: return "STAR";
        case Token::TokenType::BANG: return "BANG";
        case Token::TokenType::BANG_EQUAL: return "BANG_EQUAL";
        case Token::TokenType::EQUAL: return "EQUAL";
        case Token::TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case Token::TokenType::GREATER: return "GREATER";
        case Token::TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case Token::TokenType::LESS: return "LESS";
        case Token::TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case Token::TokenType::IDENTIFIER: return "IDENTIFIER";
        case Token::TokenType::STRING: return "STRING";
        case Token::TokenType::NUMBER: return "NUMBER";
        case Token::TokenType::AND: return "AND";
        case Token::TokenType::CLASS: return "CLASS";
        case Token::TokenType::ELSE: return "ELSE";
        case Token::TokenType::FALSE: return "FALSE";
        case Token::TokenType::FUN: return "FUN";
        case Token::TokenType::FOR: return "FOR";
        case Token::TokenType::IF: return "IF";
        case Token::TokenType::NIL: return "NIL";
        case Token::TokenType::OR: return "OR";
        case Token::TokenType::PRINT: return "PRINT";
        case Token::TokenType::RETURN: return "RETURN";
        case Token::TokenType::SUPER: return "SUPER";
        case Token::TokenType::THIS: return "THIS";
        case Token::TokenType::TRUE: return "TRUE";
        case Token::TokenType::VAR: return "VAR";
        case Token::TokenType::WHILE: return "WHILE";
        case Token::TokenType::INT: return "INT";
        case Token::TokenType::BOOL: return "BOOL";
        case Token::TokenType::FLOAT: return "FLOAT";
        case Token::TokenType::CHAR: return "CHAR";
        case Token::TokenType::DOUBLE: return "DOUBLE";
        case Token::TokenType::EOFF: return "EOFF";
    }
    // UNREACHABLE
    std::terminate();
}