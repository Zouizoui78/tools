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

    struct Lap {
        double duration;
        double global_duration;
    };

    void lap();

    template <typename Duration = std::chrono::seconds>
    double get_duration() const {
        using namespace std::chrono;
        return duration_cast<duration<double, typename Duration::period>>(
                   steady_clock::now() - _start_time_point)
            .count();
    }

    template <typename Duration = std::chrono::seconds>
    std::vector<Lap> get_laps() const {
        using namespace std::chrono;
        using duration_cast_type = duration<double, typename Duration::period>;

        std::vector<Lap> ret;
        duration_cast_type global_duration;

        Lap lap;
        lap.duration =
            duration_cast<duration_cast_type>(_laps[0] - _start_time_point)
                .count();
        lap.global_duration = lap.duration;
        ret.push_back(lap);

        for (auto it = std::next(_laps.begin()); it != _laps.end(); ++it) {
            Lap lap;
            lap.duration =
                duration_cast<duration_cast_type>(*it - *(it - 1)).count();
            lap.global_duration =
                duration_cast<duration_cast_type>(*it - _start_time_point)
                    .count();
            ret.push_back(lap);
        }

        return ret;
    }

    // Set start time point to current time and clear stored laps.
    void reset();
};

} // namespace tools::utils

#endif // STOPWATCH_HPP