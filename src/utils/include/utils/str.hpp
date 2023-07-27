#ifndef STR_HPP
#define STR_HPP

#include <optional>
#include <string>

namespace tools::str {

std::optional<int> stoi(const std::string& str);
std::optional<double> stod(const std::string& str);

} // namespace tools::str

#endif // STR_HPP