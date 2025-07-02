#ifndef STR_HPP
#define STR_HPP

#include <expected>
#include <string>

namespace tools::str {

enum class ParseError {
    OUT_OF_RANGE,
    INVALID_ARGUMENT,
    UNSUPPORTED_TYPE
};

std::expected<int, ParseError> parse_int(const std::string &str);
std::expected<double, ParseError> parse_double(const std::string &str);

} // namespace tools::str

#endif // STR_HPP
