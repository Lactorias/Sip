#ifndef PRINTER
#define PRINTER

#include <SipVariant.hpp>
#include <Expr.hpp>

#include <Scanner.hpp>

#include <fstream>
#include <initializer_list>
#include <memory>
#include <sstream>
#include <string>
#include <LoxCallable.hpp>
#include <type_traits>
#include <variant>

// helper type for the visitor #4
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };


class AST_Printer : public VisitorExpr {
public:

    virtual Object acceptExpr(Expr &expr) override;

    virtual Object acceptCall(Call &call) override;

    virtual Object acceptAssign(Assign &assign) override;

    virtual Object acceptVariable(Variable &variable) override;

    virtual Object acceptBinary(Binary &binary) override;

    virtual Object acceptLogical(Logical &logical) override;

    virtual Object acceptGrouping(Grouping &grouping) override;

    virtual Object acceptLiteral(const Literal &literal) override;

    virtual Object acceptUnary(Unary &unary) override;

    static auto resolve_to_string(const Object& obj) -> std::string;

private:
    template <typename Expr>
    auto visit_expr(Expr &&expr) -> std::string;

    template <typename... Exprs>
    auto parenthesize(std::string name, Exprs &&...exprs) -> std::string;
};

#endif // PRINTER