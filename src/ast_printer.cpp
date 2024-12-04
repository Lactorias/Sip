#include "../include/ast/Ast_Printer.hpp"
#include <Expr.hpp>
#include <Scanner.hpp>

#include <fstream>
#include <initializer_list>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>
using Object = std::variant<std::monostate, int, double, std::string>;

// helper type for the visitor #4
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

Object AST_Printer::acceptExpr(Expr &expr) {
    return expr.visit(*this); 
}

Object AST_Printer::acceptBinary(Binary &binary) {
    return parenthesize(binary.oper.lexeme, *binary.left, *binary.right);
}

Object AST_Printer::acceptGrouping(Grouping &grouping) {
    return parenthesize("group", grouping.expression);
}

Object AST_Printer::acceptLiteral(const Literal &literal) {
    return std::visit(
        overloaded{
            [](std::monostate) -> std::string { return "nil"; },
            [](int x) -> std::string { return std::to_string(x); },
            [](double d) -> std::string { return std::to_string(d); },
            [](std::string s) -> std::string { return s; },
        },
        literal.value);
}

Object AST_Printer::acceptUnary(Unary &unary) {
    return parenthesize(unary.oper.lexeme, *unary.right);
}

auto AST_Printer::resolve_to_string(const Object &obj) -> std::string {
    return std::visit(
        overloaded{[](std::monostate) -> std::string { return "nil"; },
                   [](int x) -> std::string { return std::to_string(x); },
                   [](double d) -> std::string { return std::to_string(d); },
                   [](const std::string &s) -> std::string { return s; }},
        obj);
}

template <typename Expr>
auto AST_Printer::visit_expr(Expr &&expr) -> std::string {
    if constexpr (std::is_pointer_v<std::decay_t<Expr>>) {
        return resolve_to_string(expr->visit(*this));
    } else {
        return resolve_to_string(expr.visit(*this));
    }
}

template <typename... Exprs>
auto AST_Printer::parenthesize(std::string name, Exprs &&...exprs) -> std::string {
    std::string to_return = "";
    to_return += "(" + name;
    (((to_return +=
       " " + resolve_to_string(visit_expr(std::forward<Exprs>(exprs)))),
      ...));
    to_return += ")";
    return to_return;
}

    int main() {
        Token minus(Token::TokenType::MINUS, "-", "", 1);
        Token star(Token::TokenType::STAR, "*", "", 1);

        Literal literal_123(Object(123));
        auto literal_45_67 = Literal(Object(45.67));
        auto groupint_ptr = std::make_unique<Grouping>(literal_123);
        auto unary_ptr =
            std::make_unique<Unary>(minus, std::make_unique<Literal>(45.67));

        auto expression =
            Binary(std::move(unary_ptr), star, std::move(groupint_ptr));

        AST_Printer printer;
        std::cout << printer.resolve_to_string(expression.visit(printer));
    }
