#include <Expr.hpp>

#include <Scanner.hpp>

#include <fstream>
#include <initializer_list>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>
typedef std::variant<std::monostate, int, double, std::string> Object;

// helper type for the visitor #4
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };


class AST_Printer : public Visitor<AST_Printer, std::string> {
public:
    std::string acceptBinary(Binary &binary) {
        return parenthesize(binary.oper.lexeme, *binary.left,
                            *binary.right);
    }

    std::string acceptGrouping(Grouping &grouping) {
        return parenthesize("group", grouping.expression);
    }

    std::string acceptLiteral(const Literal &literal) {
        return std::visit(overloaded{
            [](std::monostate) { return "nil"; },
            [](int x) { return std::to_string(x); },
            [](double d) { return std::to_string(d); },
            [](std::string s) { return s; },
        }, literal.value);
    }

    std::string acceptUnary(Unary &unary) {
        return parenthesize(unary.oper.lexeme, *unary.right);
    }

private:
    template <typename Expr>
    std::string visit_expr(Expr &&expr) {
        if constexpr (std::is_pointer_v<std::decay_t<Expr>>) {
            return expr->visit(*this);
        } else {
            return expr.visit(*this);
        }
    }

    template <typename... Exprs>
    std::string parenthesize(std::string name, Exprs &&...exprs) {
        std::string to_return = "";
        to_return += "(" + name;
        (((to_return += " " + visit_expr(std::forward<Exprs>(exprs))), ...));
        to_return += ")";
        return to_return;
    }
};

int main() {
    Token minus(Token::TokenType::MINUS, "-", "", 1);
    Token star(Token::TokenType::STAR, "*", "", 1);

    Literal literal_123(Object(123));
    auto literal_45_67 = Literal(Object(45.67));
    auto groupint_ptr = std::make_unique<Grouping>(literal_123);
    auto unary_ptr =
        std::make_unique<Unary>(minus, std::make_unique<Literal>(45.67));

    auto expression = Binary(std::move(unary_ptr), star, std::move(groupint_ptr));

    AST_Printer printer;
    expression.visit(printer);
}
