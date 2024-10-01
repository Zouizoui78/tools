#ifndef TIME_HPP
#define TIME_HPP

#include <chrono>
#include <string>

namespace tools::time {

std::chrono::system_clock::time_point
string_to_time(const std::string &time_str, const std::string &format);

} // namespace tools::time

#endif // TIME_HPP