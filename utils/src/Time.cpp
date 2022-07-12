#include "tools/utils/Time.hpp"

#include <ctime>

namespace tools::utils::time {

time_t string_to_time(const std::string &time, const std::string &format) {
    std::tm tm {};
    strptime(time.c_str(), format.c_str(), &tm);
    return timegm(&tm);
}

} // namespace tools::utils