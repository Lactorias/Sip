#include "../outputTests/Expr.hpp"
#include "Scanner.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

class AST_Printer : public Visitor<AST_Printer, std::string> {
public:
    std::string acceptBinary(Binary &binary) { return "hiiiiiiiiiii"; }
};

int main() {
    Binary binary;
    AST_Printer printer;

    std::string result = binary.visit(printer);

    std::cout << result << std::endl;
}
