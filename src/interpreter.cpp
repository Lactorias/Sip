#include <Interpreter.hpp>
#include <Token.hpp>
#include <variant>
#include <iostream>
#include <Ast_Printer.hpp>
#include <Expr.hpp>
#include <Stmt.hpp>
#include <RuntimeError.hpp>

Object Interpreter::acceptExpr(Expr &expr) { return nullptr; }

auto Interpreter::interpret(std::vector<unique_ptr<Stmt>> statements) -> void {
    try {
        for (auto& statement : statements) {
            execute(*statement);
        }
    } catch (RuntimeError error) {
        error.runtime_error(error);
    }
}

auto Interpreter::execute(Stmt &stmt) -> void {
    stmt.visit(*this);
}

Object Interpreter::acceptStmt(Stmt &stmt) { return 3; }

Object Interpreter::acceptBlock(Block &block) {
    execute_block(block.statements, std::make_shared<Environment>(environment));
    return std::monostate();
}

auto Interpreter::execute_block(std::vector<unique_ptr<Stmt>>& statements, std::shared_ptr<Environment> environment) -> void {
    auto prev = this->environment;
    this->environment = environment;
    try {
        for (auto &statement : statements) {
            execute(*statement);
        }
    } catch (...) {
        this->environment = prev;
        throw;
    }
    this->environment = prev; 
}


Object Interpreter::acceptAssign(Assign &assign) {
    auto value = evaluate(*assign.value);
    environment.assign(*assign.name, value);
    return value;
}


Object Interpreter::acceptBinary(Binary &binary) {
    Object left = evaluate(*binary.left);
    Object right = evaluate(*binary.right);

    switch (binary.oper->ty) {
        case Token::TokenType::MINUS:
            check_number_operand(*binary.oper, right);
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
            throw RuntimeError(*binary.oper, "Operands must be two numbers, or two strings.");
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

Object Interpreter::acceptExpression(Expression &expression) {
    evaluate(*expression.expression);
    return std::monostate();
}

Object Interpreter::acceptPrint(Print &print) {
    auto value = evaluate(*print.expression);
    if (!std::holds_alternative<std::monostate>(value)) std::cout << stringify(value) << '\n';
    return std::monostate();
}

Object Interpreter::acceptVariable(Variable &variable) {
    return environment.get(*variable.name);
}

Object Interpreter::acceptVar(Var &var) {
    Object val = std::monostate();
    if (var.initializer != nullptr) val = evaluate(*var.initializer);
    environment.define(var.name->lexeme, val);
    return std::monostate();
}

auto Interpreter::is_truth(Object &object) -> bool {
    // monostate can represent null/void
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
    if (std::holds_alternative<std::monostate>(object)) return "";
    if (std::holds_alternative<double>(object)) {
        auto text = std::to_string(extract_double(object));  
        return text;
    }
    return AST_Printer::resolve_to_string(object); 
}

auto Interpreter::check_number_operand(Token& op, Object &operand) -> void {
    if (std::holds_alternative<double>(operand)) return;
    throw RuntimeError(op, "Operand must be a number");
}

auto Interpreter::check_number_operand(Token& op, Object& left, Object& right) -> void {
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;
    throw RuntimeError(op, "Operands must be numbers");
}