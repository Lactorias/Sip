#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <fstream>

class Sip {
public:
    // A method within the class to handle the logic
    bool hadError = false;
    int run(int argc, char* argv[]){
       if (argc > 2) {
            std::cerr << "Using Sip Script" << std::endl;
            return 64;
       } else if (argc == 2){
            return runFile(argv[1]);
       } else {
            return runPrompt();
       }
    }
    void error(int line, std::string message){
        report(line, "", message);
    }

private:
    int runFile(const std::string& path){
        std::ifstream file(path);
        if (!file.is_open()){
            std::cerr << "Error: Could not open file" << path << std::endl;
            return 1;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        run(buffer.str());
        file.close();
        return 0;
    }
    int runPrompt(){
        std::string line;
        while (true){
            std::cout << "> ";
            if (!std::getline(std::cin, line)) break;
            run(line);
        }
        return 0;
    }
    void run(const std::string& source){
        std::cout << "Running: " << source << std::endl;
    }
    void report(int line, std::string where, std::string message){
        std::cerr << "[Line " << line << "]" << where << ": " << message << std::endl;
        hadError = true;
    }
};

// Define the global main function
int main(int argc, char* argv[]) {
    Sip sip;
    return sip.run(argc, argv); // Call the class method from the main function
}
