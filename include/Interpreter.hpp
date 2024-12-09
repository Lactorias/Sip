#ifndef INTERPRETER
#define INTERPRETER

#include <Expr.hpp>



/*
    To interpret our language in the early stages, we will directly execute the syntax tree itself.
*/

class Interpreter : public Visitor {
public:

    auto interpret(Expr &expr) -> void;

    virtual Object acceptExpr(Expr &expr) override;

    virtual Object acceptBinary(Binary &binary) override;

    virtual Object acceptGrouping(Grouping &grouping) override;

    virtual Object acceptLiteral(const Literal &literal) override;

    virtual Object acceptUnary(Unary &unary) override;

private:
    auto evaluate(Expr &expr) -> Object;

    auto is_truth(Object &object) -> bool;

    auto is_equal(Object &a, Object &b) -> bool;

    auto extract_double(const Object& object) -> double;

    auto stringify(Object object) -> std::string;

};

#endif // INTERPRETER
