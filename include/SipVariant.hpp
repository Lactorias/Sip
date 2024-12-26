#ifndef SIPVARIANT
#define SIPVARIANT

#include <variant>
#include <string>

struct Lox_Function;
struct Func;
struct Lox_Class;
struct Lox_Instance;


using Lox_Callable = std::variant<std::monostate, Func, Lox_Function, Lox_Class, Lox_Instance>;

using Object = std::variant<std::monostate, int, std::string, double, bool, Lox_Callable>;





#endif // SIPVARIANT