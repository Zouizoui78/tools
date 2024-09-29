#include "tools/utils/time.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>

namespace tools::time {

std::chrono::system_clock::time_point
string_to_time(const std::string& time_str, const std::string& format) {
    std::tm tm{};
    std::istringstream ss(time_str);
    ss >> std::get_time(&tm, format.c_str());
#ifdef _WIN32
    auto time = _mkgmtime(&tm);
#else
    auto time = timegm(&tm);
#endif
    return std::chrono::system_clock::from_time_t(time);
}

} // namespace tools::time