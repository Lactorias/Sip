#include <Interpreter.hpp>
#include <Token.hpp>
#include <variant>
#include <iostream>
#include <Ast_Printer.hpp>

AST_Printer use_me;

Object Interpreter::acceptExpr(Expr &expr) { return nullptr; }

auto Interpreter::interpret(Expr &expr) -> void {
    Object value = evaluate(expr);
    std::cout << stringify(value) << '\n';
}

Object Interpreter::acceptBinary(Binary &binary) {
    Object left = evaluate(*binary.left);
    Object right = evaluate(*binary.right);

    switch (binary.oper->ty) {
        case Token::TokenType::MINUS:
            return extract_double(left) - extract_double(right);
        case Token::TokenType::SLASH:
            return extract_double(left) / extract_double(right);
        case Token::TokenType::STAR:
            return extract_double(left) * extract_double(right);
        case Token::TokenType::PLUS:
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return extract_double(left) + extract_double(right);
            }
            if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                return std::get<std::string>(left) + std::get<std::string>(right);
            }
            break;
        case Token::TokenType::GREATER:
            return extract_double(left) > extract_double(right);
        case Token::TokenType::GREATER_EQUAL:
            return extract_double(left) >= extract_double(right);
        case Token::TokenType::LESS:
            return extract_double(left) < extract_double(right);
        case Token::TokenType::LESS_EQUAL:
            return extract_double(left) <= extract_double(right);
        case Token::TokenType::BANG_EQUAL:
            return !is_equal(left, right);
        case Token::TokenType::EQUAL:
            return is_equal(left, right);
    }
    return nullptr;
}

Object Interpreter::acceptGrouping(Grouping &grouping) {
    return evaluate(*grouping.expression);
}

Object Interpreter::acceptLiteral(const Literal& literal) {
    return literal.value;
}

auto Interpreter::extract_double(const Object& object) -> double {
    return std::visit(
        [](const auto& value) -> double {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_integral_v<T>) {
                return static_cast<double>(value);
            } else if constexpr (std::is_floating_point_v<T>) {
                return value;
            } else if constexpr (std::is_same_v<T, std::string>) {
                try {
                    return std::stod(value);
                } catch (...) {
                    throw std::runtime_error("Cannot convert string to double");
                }
            } else if constexpr (std::is_same_v<T, bool>) {
                return value ? 1.0 : 0.0;
            } else {
                throw std::runtime_error("Unsupported type for conversion to double");
            }
        },
    object);
}

Object Interpreter::acceptUnary(Unary &unary) {
    Object right = evaluate(*unary.right);
    switch (unary.oper->ty) {
        case Token::TokenType::BANG:
            return !is_truth(right);
        case Token::TokenType::MINUS:
            return -(extract_double(right));
    }
    // unreachable
    return nullptr;
}

auto Interpreter::evaluate(Expr &expr) -> Object {
    return expr.visit(*this);
}

auto Interpreter::is_truth(Object &object) -> bool {
    // monostate can represent null
    if (std::holds_alternative<std::monostate>(object)) return false;
    if (std::holds_alternative<bool>(object)) return std::get<bool>(object);
    return true;
}

auto Interpreter::is_equal(Object &a, Object &b) -> bool {
    if (std::holds_alternative<std::monostate>(a) && std::holds_alternative<std::monostate>(b)) return true;
    if (std::holds_alternative<std::monostate>(a)) return false;
    return a == b;
}

auto Interpreter::stringify(Object object) -> std::string {
    if (std::holds_alternative<std::monostate>(object)) return "nil";
    if (std::holds_alternative<double>(object)) {
        auto text = std::to_string(extract_double(object));  
        return text;
    }
    return use_me.resolve_to_string(object); 
}