#include <Scanner.hpp>
#include <Expr.hpp>
#include <Parser.hpp>
#include <Ast_Printer.hpp>
#include <Interpreter.hpp>

#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>

// effective LUT -> perfect hashing


/*
    run() is utilised when the compiler is executed with some parameter input, i.e a conversion occurs between user and compiler,
    or the function is provided with an input file for example.
*/ 
void run(std::string input) {
    auto interpreter = Interpreter();
    auto scanner = Scanner(input);
    auto tokens = scanner.scan_tokens();
    auto parser = Parser(tokens);
    auto expression = parser.parse();
    if (!expression) {
        std::cerr << "error : expression is null";
        return;
    }
    interpreter.interpret(*expression);
}
/*
    run_file() is utilised when the compiler is executed with a file containing some input, this is read and passed to run(),
    for processing.     
*/
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
/*
    run_prompt() is used when the compiler is executed with no inputs at all, this triggers a conversation point between the user
    and compiler, where promps are read and passed to run() for processing.
*/
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
/*
    main(), of course is the entry point to our program!
*/
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
