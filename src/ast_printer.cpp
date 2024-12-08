#include <Ast_Printer.hpp>
#include <Expr.hpp>
#include <Scanner.hpp>

#include <fstream>
#include <initializer_list>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>
#include <iostream>



Object AST_Printer::acceptExpr(Expr &expr) {
    return expr.visit(*this); 
}

Object AST_Printer::acceptBinary(Binary &binary) {
    std::cout << "hi binary here : " << binary.oper->lexeme << '\n';
    return parenthesize(binary.oper->lexeme, *binary.left, *binary.right);
}

Object AST_Printer::acceptGrouping(Grouping &grouping) {
    return parenthesize("group", *grouping.expression);
}

Object AST_Printer::acceptLiteral(const Literal &literal) {
    return std::visit(
        overloaded{
            [](std::monostate) -> std::string { return "nil"; },
            [](int x) -> std::string { return std::to_string(x); },
            [](double d) -> std::string { return std::to_string(d); },
            [](std::string s) -> std::string { return s; },
            [](bool b) -> std::string { return (b == true) ? "true" : "false"; },
            [](char c) -> std::string { return std::string(c, 1); },
            [](auto &&other) -> std::string {
                throw std::runtime_error("Unexpected type in variant Object");
            }
        },
        literal.value);
}

Object AST_Printer::acceptUnary(Unary &unary) {
    return parenthesize(unary.oper->lexeme, *unary.right);
}

auto AST_Printer::resolve_to_string(const Object &obj) -> std::string {
    return std::visit(
        overloaded{[](std::monostate) -> std::string { return "nil"; },
                   [](int x) -> std::string { return std::to_string(x); },
                   [](double d) -> std::string { return std::to_string(d); },
                   [](const std::string &s) -> std::string { return s; },
                   [](bool b) -> std::string { return (b == true) ? "true" : "false"; },
                   [](char c) -> std::string { return std::string(c, 1); },
                   [](auto &&other) -> std::string {
                        throw std::runtime_error("Unexpected type in variant Object");
                    },
                },
        obj);
}

template <typename Expr>
auto AST_Printer::visit_expr(Expr &&expr) -> std::string {
    if constexpr (std::is_pointer_v<std::decay_t<Expr>>) {
        if (!expr) {
            return "nil";
        }
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
