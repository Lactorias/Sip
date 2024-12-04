#ifndef PRINTER
#define PRINTER

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
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };


class AST_Printer : public Visitor {
public:
    Object acceptExpr(Expr &expr);

    Object acceptBinary(Binary &binary);

    Object acceptGrouping(Grouping &grouping);

    Object acceptLiteral(const Literal &literal);

    Object acceptUnary(Unary &unary);

    auto resolve_to_string(const Object& obj) -> std::string;

private:
    template <typename Expr>
    auto visit_expr(Expr &&expr) -> std::string;

    template <typename... Exprs>
    auto parenthesize(std::string name, Exprs &&...exprs) -> std::string;
};

#endif // PRINTER