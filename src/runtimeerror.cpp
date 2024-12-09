#include <RuntimeError.hpp>
#include <Token.hpp>
#include <iostream>

auto RuntimeError::runtime_error(RuntimeError error) -> void {
    std::cerr << error.what() << "\n[line :" << error.token.line << "]" << '\n';
}