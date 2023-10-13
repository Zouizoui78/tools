#ifndef TIME_HPP
#define TIME_HPP

#include <string>

namespace tools::time {

time_t string_to_time(const std::string& time, const std::string& format);

} // namespace tools::time

#endif // TIME_HPP