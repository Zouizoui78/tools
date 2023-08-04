#include "tools/utils/str.hpp"

#include <stdexcept>

namespace tools::str {

template <typename T, typename Callable>
std::optional<T> from_string(const std::string& str, Callable func) {
    try {
        return func(str);
    }
    catch (const std::invalid_argument& e) {
        return std::nullopt;
    }
    catch (const std::out_of_range& e) {
        return std::nullopt;
    }
}

std::optional<int> stoi(const std::string& str) {
    return from_string<int>(
        str,
        [](const std::string& str) { return std::stoi(str); }
    );
}

std::optional<double> stod(const std::string& str) {
    return from_string<double>(
        str,
        [](const std::string& str) { return std::stod(str); }
    );
}

} // namespace tools::str