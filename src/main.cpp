#include <Scanner.hpp>
#include "../outputTests/Expr.hpp"
#include "../include/Parser.hpp"
#include "../include/ast/Ast_Printer.hpp"

#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>

// incl <ast/Binary>]
// incl <ast/Binary>]
// incl <ast/Binary>]

// 
// 1. a folder for the tingos
// 2. fix cmake
// 3. fix include
// add more binary optiosn??

// Scannner -> split
// Sip -> runs the compiler -> not templated
// token -> not temp but simple
//
// ast -> this is the one place, where we haev a real "folder"
// split this into its  own compilation unit
// 
// printer 
//
//                                                            setup ast folder
//
// errorhandler

// effective LUT -> perfect hashing

/*
    include/
        ast/

    src/
        
*/

void run(std::string input) {
    auto scanner = Scanner(input);
    auto tokens = scanner.scan_tokens();
    auto parser = Parser(tokens);
    auto expression = make_unique<Expr>(parser.parse());
    auto printer = AST_Printer(); 
    std::cout << printer.acceptExpr(expression); 
}

int run_file(const std::string &path) {
    auto file = std::ifstream(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file" << path << std::endl;
        return 1;
    }

    auto buffer = std::stringstream{};
    buffer << file.rdbuf();
    run(buffer.str());
    file.close();

    return 0;
}

int run_prompt() {
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line))
            break;
        run(line);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        std::cerr << "Using Sip Script" << std::endl;
        return 64;
    } else if (argc == 2) {
        return run_file(argv[1]);
    } else {
        return run_prompt();
    }
}
