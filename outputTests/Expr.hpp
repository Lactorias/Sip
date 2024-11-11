#include "../src/Token.hpp"
#include <any>
#include <memory>
#include <utility>
#include <variant>
typedef std::variant<std::monostate, int, std::string> Object;

class Binary;
class Grouping;
class Literal;
class Unary;

// using Expr = std::variant<std::monostate, Binary, Grouping, Literal, Unary>;

template <typename VisitorType, typename T, typename... Args>
class Visitor {
public:
    T acceptBinary(Binary &Binary, Args &&...args) {
        return static_cast<VisitorType *>(this)->acceptBinary(Binary, args...);
    }
};

template <typename Derived>
class Expr {
public:
    template <typename Visitor, typename... Args>
    auto visit(Visitor &visitor, Args &&...args) {
        return static_cast<Derived *>(this)->visit(visitor,
                                                   std::forward<Args>(args)...);
    };
};

class Binary : public Expr<Binary> {
public:
    Binary() : left(nullptr), oper(Token()), right(nullptr) {}

    // Binary(std::unique_ptr<Expr> left, const Token oper,
    // std::unique_ptr<Expr> right)
    //    : left(left), oper(oper), right(right) {}

    template <typename Visitor, typename... Args>
    auto visit(Visitor &visitor, Args &&...args) {
        return visitor.acceptBinary(*this, std::forward<Args>(args)...);
    }

    std::unique_ptr<Expr> left;
    const Token oper;
    std::unique_ptr<Expr> right;
};
