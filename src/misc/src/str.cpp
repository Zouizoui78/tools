#include "tools/misc/str.hpp"

#include <stdexcept>

namespace tools::str {

template <typename T>
std::expected<T, ParseError> parse_number(const std::string &str) {
    try {
        if constexpr (std::is_same_v<T, int>) {
            return std::stoi(str);
        }
        else if constexpr (std::is_same_v<T, double>) {
            return std::stod(str);
        }
        else {
            return std::unexpected(ParseError::UNSUPPORTED_TYPE);
        }
    }
    catch (const std::invalid_argument &e) {
        return std::unexpected(ParseError::INVALID_ARGUMENT);
    }
    catch (const std::out_of_range &e) {
        return std::unexpected(ParseError::OUT_OF_RANGE);
    }
}

std::expected<int, ParseError> parse_int(const std::string &str) {
    return parse_number<int>(str);
}

std::expected<double, ParseError> parse_double(const std::string &str) {
    return parse_number<double>(str);
}

} // namespace tools::str
