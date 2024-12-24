#include <Interpreter.hpp>
#include <Token.hpp>
#include <variant>
#include <iostream>
#include <Ast_Printer.hpp>
#include <LoxCallable.hpp>
#include <memory>
#include <Expr.hpp>
#include <Stmt.hpp>
#include <RuntimeError.hpp>
#include <Return.hpp>

Object Interpreter::acceptExpr(Expr &expr) { return std::monostate(); }

auto Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> statements) -> void {
    try {
        for (auto& statement : statements) {
            execute(statement);
        }
    } catch (RuntimeError error) {
        error.runtime_error(error);
    }
}

auto Interpreter::execute(std::shared_ptr<Stmt> stmt) -> void {
    if (stmt) stmt->visit(*this);
    //stmt->visit(*this);
    else std::cout << "nah" << '\n';
}

Object Interpreter::acceptCall(Call &call) {
    auto callee = evaluate(*call.callee);
    vector<Object> arguments;
    for (auto& argument : call.arguments) {
        arguments.push_back(evaluate(*argument));
    }
    auto obj = callee;
    if (!std::holds_alternative<Lox_Callable>(obj)) return std::monostate{};
    auto function = std::get<Lox_Callable>(obj);
    if (arguments.size() != Arity{}(function)) {
        throw RuntimeError(*call.paren, "Expected " + std::to_string(std::get<Lox_Function>(function).arity()) + " arguments but got " + std::to_string(arguments.size()) + ".");
    }
    return Callee{.interpreter = *this, .args = arguments}(function);
}

Object Interpreter::acceptFunction(Function &function) {
    auto func = std::make_shared<Lox_Function>(function, environment);
    environment->define(function.name->lexeme, *func);
    return {};
}

Object Interpreter::accept_Return(_Return &_return) {
    throw evaluate(*_return.value);
}

Object Interpreter::acceptStmt(Stmt &stmt) { return 3; }

Object Interpreter::acceptBlock(Block &block) {
    execute_block(block.statements, environment);
    return std::monostate();
}

Object Interpreter::accept_If(_If &_if) {
    if (is_truth(evaluate(*_if.condition))) {
        execute(_if.then_branch);
    } else if (_if.else_branch != nullptr) {
        execute(_if.else_branch);
    }
    return std::monostate();
}

Object Interpreter::accept_While(_While &_while) {
    while (is_truth(evaluate(*_while.condition))) {
        execute(_while.body);
    }
    return std::monostate();
}

Object Interpreter::acceptLogical(Logical &logical) {
    auto left = evaluate(*logical.left);
    if (logical.oper->ty == Token::TokenType::OR) {
        if (is_truth(left)) return left;
    } else {
        if (!is_truth(left)) return left;
    }
    return evaluate(*logical.right);
}

auto Interpreter::execute_block(const std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> environment) -> void {
    auto prev = this->environment;
    this->environment = environment;
    try {
        for (auto &statement : statements) {
            if (!statement) std::cout << "NULLLLLLLLLLLLLLLLLLLLLLLL" << '\n';
            execute(statement);
        }
    } catch (...) {
        this->environment = prev;
        throw;
    }
    this->environment = prev; 
}


Object Interpreter::acceptAssign(Assign &assign) {
    auto value = evaluate(*assign.value);
    environment->assign(*assign.name, value);
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
    return std::monostate();
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
    return std::monostate();
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
    return environment->get(*variable.name);
}

Object Interpreter::acceptVar(Var &var) {
    Object val = std::monostate();
    if (var.initializer != nullptr) val = evaluate(*var.initializer);
    environment->define(var.name->lexeme, val);
    return std::monostate();
}

auto Interpreter::is_truth(const Object &object) -> bool {
    // monostate can represent null/void
    if (std::holds_alternative<std::monostate>(object)) return false;
    if (std::holds_alternative<bool>(object)) return std::get<bool>(object);
    return true;
}

auto Interpreter::is_equal(Object &a, Object &b) -> bool {
    if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) return std::get<double>(a) == std::get<double>(b);
    if (std::holds_alternative<std::string>(a) && std::holds_alternative<std::string>(b)) return std::get<std::string>(a) == std::get<std::string>(b);
    if (std::holds_alternative<bool>(a) && std::holds_alternative<bool>(b)) return std::get<bool>(a) == std::get<bool>(b);
    // come back to this one
    if (std::holds_alternative<std::monostate>(a) && std::holds_alternative<std::monostate>(b)) return true;
    return false;
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