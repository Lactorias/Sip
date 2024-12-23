#ifndef SIPVARIANT
#define SIPVARIANT

#include <variant>
#include <string>

struct Lox_Function;
struct Func;

using Lox_Callable = std::variant<std::monostate, Func, Lox_Function>;
using Object = std::variant<std::monostate, int, std::string, double, bool, Lox_Callable>;







#endif // SIPVARIANT