#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <chrono>

namespace tools::utils {

class Stopwatch {

    public:
    
    Stopwatch(const std::string &name = "");

    ~Stopwatch();

    private:

    double get_duration() const;
    void log(double d) const;

    std::string _name = "";

    std::chrono::steady_clock::time_point _start_time_point;
};

} // namespace tools::utils

#endif // STOPWATCH_HPP