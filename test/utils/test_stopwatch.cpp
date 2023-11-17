#include "tools/utils/stopwatch.hpp"
#include "gtest/gtest.h"

#include <numeric>

namespace test {

using namespace tools::utils;
using namespace std::chrono;
using namespace std::chrono_literals;

TEST(TestStopwatch, test_stopwatch) {
    Stopwatch s;
    auto stop_time = s.get_duration() + 1ms;
    while (s.get_duration() < stop_time)
        ;
    auto d = s.get_duration();
    std::cout << s.get_duration() << std::endl;
    ASSERT_TRUE(900us < d && d < 1100us);
}

TEST(TestStopwatch, test_stopwatch_laps) {
    Stopwatch s;
    auto next_lap = s.get_duration() + 1ms;
    for (int i = 0; i < 10; ++i) {
        while (s.get_duration() < next_lap)
            ;
        s.lap();
        next_lap = s.get_duration() + 1ms;
    }

    auto laps = s.get_laps<microseconds>();
    auto mean = std::accumulate(laps.begin(), laps.end(), 0us) / laps.size();
    ASSERT_TRUE(900us < mean && mean < 1100us);
}

} // namespace test