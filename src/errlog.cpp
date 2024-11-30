#include <ErrLog.hpp>

auto ErrLog::report(size_t line, std::string error) -> void {
    logs_.emplace_back(line, std::move(error));
}

auto ErrLog::empty() const noexcept -> bool { return logs_.empty(); }

// err << os

// l, r
// 

// inside L
// l.<<(r)
// l << r
