#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

auto split(const std::string &s, const std::string &delimiter)
    -> std::vector<std::string> {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = 0;
    while ((end = s.find(delimiter, start)) != std::string::npos) {
        result.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
    }
    result.push_back(s.substr(start));
    return result;
}

auto to_upper_str(const std::string& input) noexcept -> std::string {
    auto result = input;
    std::transform(result.begin(), result.end(), result.begin(), [](char c){
        return toupper(c);
    });
    return result;
}

auto to_lower_str(const std::string& input) noexcept -> std::string {
    auto result = input;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c){
        return tolower(c);
    });
    return result;
} 

void defineType(std::ofstream &writer, const std::string &baseName,
                const std::string &className, const std::string &fieldList) {
    writer << "\n";
    writer << "class " << className << " : public " << baseName << " {" << std::endl;
    writer << "public:" << std::endl;

    // Constructor
    writer << "    " << className << "(" << fieldList << ") :";

    // Parsing and storing fields
    auto fields = split(fieldList, ", ");
    for (size_t i = 0; i < fields.size(); i++) {
        auto field = fields[i];
        // Trim whitespace around field
        field.erase(0, field.find_first_not_of(" \t"));
        field.erase(field.find_last_not_of(" \t") + 1);

        // Find the last space in the field to get the variable name
        size_t nameStart = field.find_last_of(' ') + 1;
        std::string name = field.substr(nameStart);

        if (i != fields.size() - 1) {
            writer << " " << name << "(std::move(" << name << ")),";
        } else {
            writer << " " << name << "(std::move(" << name << "))";
        }
    }
    writer << " {}" << std::endl;

    // Visit function

    writer << '\n';

    writer << "    Object visit (Visitor" << baseName << " &visitor) override {" << '\n';
    writer << "        return visitor.accept" << className << "(*this);" << '\n';
    writer << "    }" << '\n' << '\n';

    // Fields
    for (const auto &field : fields) {
        writer << "    " << field << ";" << std::endl;
    }
    writer << "};" << std::endl;
}
// Functioni to define the AST
void defineAST(const std::string &outputDir, const std::string &baseName,
               const std::vector<std::string> &types) {
    auto path = outputDir + "/" + baseName + ".hpp";
    std::ofstream writer(path, std::ios::out | std::ios::binary);
    if (!writer.is_open()) {
        std::cerr << "Error: Could not open the file for writing at " << path
                  << std::endl;
        return;
    }
    writer << "#ifndef " << to_upper_str(baseName) << '\n';
    writer << "#define " << to_upper_str(baseName) << '\n';
    writer << "#include <Token.hpp>" << std::endl;
    writer << "#include <Expr.hpp>" << '\n';
    writer << "#include <variant>" << std::endl;
    writer << "#include <memory>" << '\n';
    writer << "#include <utility>" << '\n';
    writer << "using Object = std::variant<std::monostate, int, std::string, double, bool>;"
           << std::endl;
    writer << "using std::unique_ptr;" << '\n';

    writer << std::endl;

    for (auto const& type : types) {
        auto type_parts = split(type, ":");
        auto class_name = type_parts[0];
        class_name.erase(0, class_name.find_first_not_of(" \t"));
        class_name.erase(class_name.find_last_not_of(" \t") + 1);
        writer << "class " << class_name << ";" << '\n';
    }
    writer << "class " << baseName << ";" << '\n';
    writer << '\n';

    writer << '\n';
    writer << "class Visitor" << baseName << " {" << '\n';
    writer << "public:" << '\n';
// neeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeed to add const to literal! also the namespaces
    for (auto const& type : types) {
        auto type_parts = split(type, ":");
        auto class_name = type_parts[0];
        class_name.erase(0, class_name.find_first_not_of(" \t"));
        class_name.erase(class_name.find_last_not_of(" \t") + 1);
        if (class_name == "Literal") writer <<  "    virtual Object accept" << class_name << "(" << "const " << class_name << " &" << to_lower_str(class_name) << ") = 0;" << '\n' << '\n'; 
        else writer <<  "    virtual Object accept" << class_name << "(" << class_name << " &" << to_lower_str(class_name) << ") = 0;" << '\n' << '\n';
    }
    writer <<  "    virtual Object accept" << baseName << "(" << baseName << " &" << to_lower_str(baseName) << ") = 0;" << '\n' << '\n';
    writer << "};" << '\n';

    writer << "class " <<  baseName << " {" << '\n';
    writer << "public:" << '\n';
    writer << "    virtual Object visit(Visitor" << baseName << " &visitor) = 0;" << '\n';
    writer << "};" << '\n';

    for (const auto &type : types) {
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

    writer << '\n' << '\n';
    writer << "#endif // " << to_upper_str(baseName) << '\n';
}

// Main function to execute the AST generation
auto main(int argc, char *argv[]) -> int {
    if (argc != 2) {
        std::cerr << "Usage: ./ast_generate <output directory>" << std::endl;
        return 64;
    }
    auto outputDir = argv[1]; // Argument for output directory
    defineAST(outputDir, "Expr",
              std::vector<std::string>{
                  "Binary   : unique_ptr<Expr> left, unique_ptr<Token> oper, "
                  "unique_ptr<Expr> right",
                  "Grouping : unique_ptr<Expr> expression",
                  "Literal  : const Object value",
                  "Unary    : unique_ptr<Token> oper, unique_ptr<Expr> right",
                  "Variable : unique_ptr<Token> name",
              });
    defineAST(outputDir, "Stmt", 
              std::vector<std::string>{
                 "Expression : unique_ptr<Expr> expression",
                 "Print      : unique_ptr<Expr> expression",
                 "Var        : unique_ptr<Token> name, unique_ptr<Expr> initializer"
              });
    return 0;
}
