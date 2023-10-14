#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <chrono>
#include <string>

namespace tools::utils {

class Stopwatch {
private:
    std::chrono::steady_clock::time_point _start_time_point;

public:
    Stopwatch();

    template <typename DurationType>
    DurationType get_duration() const {
        return std::chrono::duration_cast<DurationType>(
            std::chrono::steady_clock::now() - _start_time_point);
    }

    void reset();
};

} // namespace tools::utils

#endif // STOPWATCH_HPP