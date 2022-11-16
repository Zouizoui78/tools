#include "tools/utils/Time.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>

namespace tools::utils::time {

time_t string_to_time(const std::string &time, const std::string &format) {
    std::tm tm {};
    std::istringstream ss(time);
    ss >> std::get_time(&tm, format.c_str());
#ifdef LINUX
    return timegm(&tm);
#elif WINDOWS
    return _mkgmtime(&tm);
#endif
}

} // namespace tools::utils