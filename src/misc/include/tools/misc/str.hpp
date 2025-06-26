#ifndef STR_HPP
#define STR_HPP

#include <expected>
#include <string>

namespace tools::str {

enum class ErrorCode {
    OUT_OF_RANGE,
    INVALID_ARGUMENT,
    UNSUPPORTED_TYPE
};

std::expected<int, ErrorCode> stoi(const std::string &str);
std::expected<double, ErrorCode> stod(const std::string &str);

} // namespace tools::str

#endif // STR_HPP
