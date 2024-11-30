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

class AST_Printer : public Visitor<AST_Printer, std::string> {
public:
    std::string acceptBinary(Binary &binary) {
        return parenthesize(binary.oper.lexeme, binary.left.get(),
                            binary.right.get());
    }

    std::string acceptGrouping(Grouping &grouping) {
        return parenthesize("group", grouping.expression);
    }

    std::string acceptLiteral(const Literal &literal) {
        return std::visit(
            [](const auto &arg) -> std::string {
                if constexpr (std::is_same_v<std::decay_t<decltype(arg)>,
                                             std::string>) {
                    return arg;
                } else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>,
                                                    std::monostate>) {
                    return "nil";
                } else {
                    return std::to_string(arg);
                }
            },
            literal.value);
    }

    std::string acceptUnary(Unary &unary) {
        return parenthesize(unary.oper.lexeme, unary.right.get());
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
    Token minus(TokenType::MINUS, "-", "", 1);
    Token star(TokenType::STAR, "*", "", 1);

    Literal literal_123(Object(123));
    auto literal_45_67 = Literal(Object(45.67));
    auto groupint_ptr = std::make_unique<Grouping>(literal_123);
    auto unary_ptr =
        std::make_unique<Unary>(minus, std::make_unique<Literal>(45.67));
    Binary expression(unary_ptr, star, groupint_ptr);
    AST_Printer printer;
    expression.visit(printer);
}
