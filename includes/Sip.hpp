#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <fstream>

class Sip {
public:
    // A method within the class to handle the logic
    bool hadError;
    int run(int argc, char* argv[]);
    void error(int line, std::string message);
private:
    int runFile(const std::string& path);
    int runPrompt();
    void run(const std::string& source);
    void report(int line, std::string where, std::string message);
};

