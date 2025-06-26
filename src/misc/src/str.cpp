#include "tools/misc/str.hpp"

#include <stdexcept>
#include <type_traits>

namespace tools::str {

template <typename T>
std::expected<T, ErrorCode> from_string(const std::string &str) {
    try {
        if constexpr (std::is_same_v<T, int>) {
            return std::stoi(str);
        }
        else if constexpr (std::is_same_v<T, double>) {
            return std::stod(str);
        }
        else {
            return std::unexpected(ErrorCode::UNSUPPORTED_TYPE);
        }
    }
    catch (const std::invalid_argument &e) {
        return std::unexpected(ErrorCode::INVALID_ARGUMENT);
    }
    catch (const std::out_of_range &e) {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }
}

std::expected<int, ErrorCode> stoi(const std::string &str) {
    return from_string<int>(str);
}

std::expected<double, ErrorCode> stod(const std::string &str) {
    return from_string<double>(str);
}

} // namespace tools::str
