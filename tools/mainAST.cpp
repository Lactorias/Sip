#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Split function to divide strings by a delimiter
auto split(std::string s, const std::string& delimiter) -> std::vector<std::string> {
    auto to_return = std::vector<std::string>();
    size_t pos = 0;
    std::string part;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        part = s.substr(0, pos);
        to_return.push_back(part);
        s.erase(0, pos + delimiter.length());
    }
    to_return.push_back(s);
    return to_return;
}


// Function to define a class based on base name, class name, and field list
void defineType(std::ofstream& writer, const std::string& baseName, const std::string& className, const std::string& fieldList) {
    // Header
       
    writer << "class " << className << " : " << "public " << baseName << " {" << std::endl;
    writer << "public:" << std::endl;
    
    // Constructor
    writer << "    " << className << "(" << fieldList << ") :";

    // Store parameters
    auto fields = split(fieldList, ", ");
    for (size_t i = 0; i < fields.size(); i++) {
        const auto fieldParts = split(fields[i], " ");
        const auto name = fieldParts[2];  // Get the field name
        if (i != fields.size() - 1) {
            writer << " " << name << "(" << name << "),";
        } else {
            writer << " " << name << "(" << name << ")";
        }
    }
    writer << " {}" << std::endl;

    // Fields
    for (const auto& field : fields) {
        writer << "    " << field << ";" << std::endl;
    }
    writer << "};" << std::endl;
    writer << std::endl;
}

// Function to define the AST
void defineAST(const std::string& outputDir, const std::string& baseName, const std::array<std::string, 4>& types) {
    auto path = outputDir + "/" + baseName + ".cpp";
    std::ofstream writer(path, std::ios::out | std::ios::binary);
    if (!writer.is_open()) {
	std::cerr << "Error: Could not open the file for writing at " << path << std::endl;
	return;
    }
    writer << "#include \"../src/Token.hpp\"" << std::endl;
    writer << "#include <memory>" << std::endl;
    writer << std::endl;
    writer << "class " << baseName << "{" << std::endl;
    writer << "public: " << std::endl;
    writer << "     " << baseName << "& " << "operator = (const " << baseName << "& other) = delete;" << std::endl;
    writer << "     " << baseName << "() = default;" << std::endl;
    writer << "     " << baseName << "(" << baseName << "& other) = delete;" << std::endl;
    writer << "     virtual ~Expr() = default;" << std::endl;
    writer << "};" << std::endl;
    writer << std::endl;
    
    for (const auto& type : types) {
        auto typeParts = split(type, ":");
        if (typeParts.size() == 2) {
            auto className = typeParts[0];
            auto fields = typeParts[1];
            className.erase(0, className.find_first_not_of(" \t"));
            className.erase(className.find_last_not_of(" \t") + 1);
            fields.erase(0, fields.find_first_not_of(" \t"));
            fields.erase(fields.find_last_not_of(" \t") + 1);
            defineType(writer, baseName, className, fields);
        }
    }
}

// Main function to execute the AST generation
auto main(int argc, char* argv[]) -> int {
    if (argc != 2) {
        std::cerr << "Usage: ./ast_generate <output directory>" << std::endl;
        return 64;
    }
    auto outputDir = argv[1];  // Argument for output directory
    defineAST(outputDir, "Expr", std::array<std::string, 4>{
        "Binary   : const Expr& left, const Token& oper, const Expr& right",
        "Grouping : const Expr& expression",
        "Literal  : const std::variant<std::monostate, int, std::string>& value",
        "Unary    : const Token& oper, const Expr& right",
    });
    return 0;
}
