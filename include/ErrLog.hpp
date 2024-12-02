#ifndef ERRORLOG
#define ERRORLOG

#include <string>
#include <vector>
#include <utility>
#include <Token.hpp>

#include <iostream>

struct ErrLog {
public:
    auto report(size_t line, std::string error) -> void;
    auto empty() const noexcept -> bool;
    auto error(Token token, std::string message) noexcept -> void;

    friend auto operator<<(std::ostream &os, ErrLog const& errlog) -> std::ostream& {
        for (auto const& [line, error] : errlog.logs_) {
            os << line << ": " << error << '\n';
        }
        return os;
    }

private:
    std::vector<std::pair<size_t, std::string>> logs_;
};


#endif // ERRORLOG