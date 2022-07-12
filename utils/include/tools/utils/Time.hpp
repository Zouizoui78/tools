#ifndef TIME_HPP
#define TIME_HPP

#include <string>

namespace tools::utils::time {

time_t string_to_time(const std::string &time, const std::string &format);

} // namespace tools::utils

#endif // TIME_HPP