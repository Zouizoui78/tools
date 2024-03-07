#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <chrono>
#include <vector>

namespace tools::utils {

class Stopwatch {
private:
    std::chrono::steady_clock::time_point _start_time_point;
    std::vector<std::chrono::steady_clock::time_point> _laps;

public:
    Stopwatch();

    void lap();

    template <typename DurationType = std::chrono::nanoseconds>
    requires std::is_convertible_v<DurationType, std::chrono::nanoseconds>
    DurationType get_duration() const {
        return std::chrono::duration_cast<DurationType>(
            std::chrono::steady_clock::now() - _start_time_point);
    }

    template <typename DurationType = std::chrono::nanoseconds>
    requires std::is_convertible_v<DurationType, std::chrono::nanoseconds>
    std::vector<DurationType> get_laps() const {
        std::vector<DurationType> ret;
        ret.emplace_back(std::chrono::duration_cast<DurationType>(
            _laps[0] - _start_time_point));
        for (auto it = std::next(_laps.begin()); it != _laps.end(); ++it) {
            ret.emplace_back(
                std::chrono::duration_cast<DurationType>(*it - *(it - 1)));
        }
        return ret;
    }

    // Set start time point to current time and clear internal laps vector.
    void reset();
};

} // namespace tools::utils

#endif // STOPWATCH_HPP