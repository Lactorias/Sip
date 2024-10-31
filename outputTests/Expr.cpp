#include "../src/Token.hpp"
#include <any>
#include <memory>
#include <string>
#include <variant>
typedef std::variant<std::monostate, int, std::string> Object;

template<typename... Types>
class Visitor;


template <typename T>
class Visitor<T> {
public:
     virtual void visit(T & visitable) = 0;
};

template <typename T, typename... Types>
class Visitor<T, Types...> : public Visitor<Types...> {
public:
    using Visitor<Types...>::visit;

    virtual void visit(T & visitable) = 0;
};

template <typename... Types>
class Visitable {
public:
    virtual void accept(Visitor<Types...>& visitor) = 0;

    virtual ~Visitable<Types...>() = default;
};

template<typename Derived, typename... Types>
class Expr : public Visitable<Types...>{
public:
     virtual void accept(Visitor<Types...>& visitor) override {
         visitor.visit(static_cast<Derived&>(*this));
     }

     virtual ~Expr() = default;
     Expr(const Expr& other) = delete;
     Expr& operator=(const Expr& other) = delete;
     Expr() = default;
};

class Binary : public Expr<Binary, int, std::string> {
public:
    Binary(const Expr& left, const Token& oper, const Expr& right) : left(left), oper(oper), right(right) {}

      accept(Visitor<std::any> visitor) override {
         return visitor.visit(this);
     }
    const Expr& left;
    const Token& oper;
    const Expr& right;
};

class Grouping : public Expr {
public:
    Grouping(const Expr& expression) : expression(expression) {}

     <R> R accept(Visitor<R> visitor) override {
         return visitor->visitGroupingExpr(this);
     }
    const Expr& expression;
};

class Literal : public Expr {
public:
    Literal(const Object& value) : value(value) {}

     <R> R accept(Visitor<R> visitor) override {
         return visitor->visitLiteralExpr(this);
     }
    const Object& value;
};

class Unary : public Expr {
public:
    Unary(const Token& oper, const Expr& right) : oper(oper), right(right) {}

     <R> R accept(Visitor<R> visitor) override {
         return visitor->visitUnaryExpr(this);
     }
    const Token& oper;
    const Expr& right;
};

