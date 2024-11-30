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

void defineType(std::ofstream &writer, const std::string &baseName,
                const std::string &className, const std::string &fieldList) {
    writer << "\n";
    writer << "class " << className << " : public " << baseName << "<"
           << className << "> {" << std::endl;
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
            writer << " " << name << "(" << name << "),";
        } else {
            writer << " " << name << "(" << name << ")";
        }
    }
    writer << " {}" << std::endl;

    // Visit function
    writer << "    template<typename Visitor, typename... Args>" << '\n';
    writer << "    auto visit(Visitor &visitor, Args &&...args) {" << '\n';
    writer << "        return visitor.accept" << className
           << "(*this, std::forward<Args>(args)...);" << '\n';
    writer << "    }" << '\n';

    // Fields
    for (const auto &field : fields) {
        writer << "    " << field << ";" << std::endl;
    }
    writer << "};" << std::endl;
}
// Functioni to define the AST
void defineAST(const std::string &outputDir, const std::string &baseName,
               const std::array<std::string, 4> &types) {
    auto path = outputDir + "/" + baseName + ".hpp";
    std::ofstream writer(path, std::ios::out | std::ios::binary);
    if (!writer.is_open()) {
        std::cerr << "Error: Could not open the file for writing at " << path
                  << std::endl;
        return;
    }
    writer << "#include \"../src/Token.hpp\"" << std::endl;
    writer << "#include <variant>" << std::endl;
    writer << "#include <memory>" << '\n';
    writer << "#include <utility>" << '\n';
    writer << "typedef std::variant<std::monostate, int, std::string> Object;"
           << std::endl;
    writer << "using std::unique_ptr;" << '\n';

    writer << std::endl;
    writer << "class Binary;" << '\n';
    writer << "class Grouping;" << '\n';
    writer << "class Literal;" << '\n';
    writer << "class Unary;" << '\n';
    writer << '\n';
    writer << "template <typename VisitorType, typename T, typename... Args>"
           << '\n';
    writer << "class Visitor {" << '\n';
    writer << "public:" << '\n';
    writer << "    T acceptBinary(Binary &binary, Args &&...args) {" << '\n';
    writer << "        return static_cast<VisitorType "
              "*>(this)->acceptBinary(binary, args...);"
           << '\n';
    writer << "    }" << '\n';
    writer << "    T acceptGrouping(Grouping &grouping, Args &&...args) {"
           << '\n';
    writer << "        return static_cast<VisitorType "
              "*>(this)->acceptGrouping(grouping, args...);"
           << '\n';
    writer << "    }" << '\n';
    writer << "    T acceptLiteral(Literal &literal, Args &&...args) {" << '\n';
    writer << "        return static_cast<VisitorType "
              "*>(this)->acceptLiteral(literal, args...);"
           << '\n';
    writer << "    }" << '\n';
    writer << "    T acceptUnary(Unary &unary, Args &&...args) {" << '\n';
    writer << "        return static_cast<VisitorType "
              "*>(this)->acceptUnary(unary, args...);"
           << '\n';
    writer << "    }" << '\n';
    writer << "};" << '\n';

    writer << '\n';
    writer << "template <typename Derived>" << '\n';
    writer << "class " << baseName << " {" << '\n';
    writer << "public:" << '\n';
    writer << "    template <typename Visitor, typename... Args>" << '\n';
    writer << "    auto visit(Visitor &visitor, Args &&...args) {" << '\n';
    writer << "        return static_cast<Derived *>(this)->visit(visitor, "
              "std::forward<Args>(args)...);"
           << '\n';
    writer << "    }" << '\n';
    writer << "};";
    writer << '\n';

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
}

// Main function to execute the AST generation
auto main(int argc, char *argv[]) -> int {
    if (argc != 2) {
        std::cerr << "Usage: ./ast_generate <output directory>" << std::endl;
        return 64;
    }
    auto outputDir = argv[1]; // Argument for output directory
    defineAST(outputDir, "Expr",
              std::array<std::string, 4>{
                  "Binary   : unique_ptr<Expr>& left, const Token& oper, "
                  "const Expr& right",
                  "Grouping : const Expr& expression",
                  "Literal  : const Object& value",
                  "Unary    : const Token& oper, const Expr& right",
              });
    return 0;
}
