#include <Token.hpp>
#include <cstdint>
#include <memory>
#include <utility>
#include <variant>
typedef std::variant<std::monostate, int, double, std::string> Object;
using std::unique_ptr;

class Binary;
class Grouping;
class Literal;
class Unary;

template <typename VisitorType, typename T, typename... Args>
class Visitor {
public:
    T acceptBinary(Binary &binary, Args &&...args) {
        return static_cast<VisitorType *>(this)->acceptBinary(binary, args...);
    }
    T acceptGrouping(Grouping &grouping, Args &&...args) {
        return static_cast<VisitorType *>(this)->acceptGrouping(grouping,
                                                                args...);
    }
    T acceptLiteral(Literal &literal, Args &&...args) {
        return static_cast<VisitorType *>(this)->acceptLiteral(literal,
                                                               args...);
    }
    T acceptUnary(Unary &unary, Args &&...args) {
        return static_cast<VisitorType *>(this)->acceptUnary(unary, args...);
    }
};

template <typename Derived>
class Expr {
public:
    template <typename Visitor, typename... Args>
    auto visit(Visitor &visitor, Args &&...args) {
        return static_cast<Derived *>(this)->visit(visitor,
                                                   std::forward<Args>(args)...);
    }
};

class Binary : public Expr<Binary> {
public:
    template <typename Expr_left, typename Expr_right>
    Binary(unique_ptr<Expr_left> &left, const Token &oper,
           unique_ptr<Expr_right> &right)
        : left(left), oper(oper), right(right) {}
    template <typename Visitor, typename... Args>
    auto visit(Visitor &visitor, Args &&...args) {
        return visitor.acceptBinary(*this, std::forward<Args>(args)...);
    }
    unique_ptr<Expr> &left;
    const Token &oper;
    unique_ptr<Expr> &right;
};

class Grouping : public Expr<Grouping> {
public:
    template <typename Expr_>
    Grouping(Expr_ &expression) : expression(expression) {}
    template <typename Visitor, typename... Args>
    auto visit(Visitor &visitor, Args &&...args) {
        return visitor.acceptGrouping(*this, std::forward<Args>(args)...);
    }
    Expr &expression;
};

class Literal : public Expr<Literal> {
public:
    Literal(Object value) : value(value) {}
    template <typename Visitor, typename... Args>
    auto visit(Visitor &visitor, Args &&...args) {
        return visitor.acceptLiteral(*this, std::forward<Args>(args)...);
    }
    const Object value;
};

class Unary : public Expr<Unary> {
public:
    template <typename Expr_>
    Unary(const Token oper, unique_ptr<Expr_> &&right)
        : oper(oper), right(std::move(right)) {}
    template <typename Visitor, typename... Args>
    auto visit(Visitor &visitor, Args &&...args) {
        return visitor.acceptUnary(*this, std::forward<Args>(args)...);
    }
    const Token oper;
    unique_ptr<Expr> right;
};
