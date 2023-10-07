#include "tools/utils/time.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>

namespace tools::time {

time_t string_to_time(const std::string &time, const std::string &format) {
    std::tm tm {};
    std::istringstream ss(time);
    ss >> std::get_time(&tm, format.c_str());
#ifdef WIN32
    return _mkgmtime(&tm);
#else
    return timegm(&tm);
#endif
}

} // namespace tools::time