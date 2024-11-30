#include <Scanner.hpp>
#include <iostream>
#include <ostream>
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
//
//
// errorhandler

// effective LUT -> perfect hashing

/*
    include/
        ast/
        ..
    src/
        
*/

void run(std::string input) {
    auto scanner = Scanner(input);
    auto tokens = scanner.scan_tokens();
    for (auto &token : tokens) {
        std::cout << token.ty << ": " << token << std::endl;
    }
}

int runFile(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file" << path << std::endl;
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    run(buffer.str());
    file.close();
    return 0;
}
int runPrompt() {
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
        return runFile(argv[1]);
    } else {
        return runPrompt();
    }
}
