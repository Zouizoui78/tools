#include "tools/utils/stopwatch.hpp"
#include "gtest/gtest.h"

#include <numeric>

namespace test {

using namespace tools::utils;
using namespace std::chrono;
using namespace std::chrono_literals;

TEST(TestStopwatch, test_stopwatch) {
    Stopwatch s;
    auto d = s.get_duration<nanoseconds>();
    ASSERT_LE(d, 1us);
}

TEST(TestStopwatch, test_stopwatch_laps) {
    Stopwatch s;
    auto next_lap = s.get_duration<microseconds>() + 1ms;
    for (int i = 0; i < 10; ++i) {
        while (s.get_duration<microseconds>() < next_lap)
            ;
        s.lap();
        next_lap = s.get_duration<microseconds>() + 1ms;
    }

    auto laps = s.get_laps<microseconds>();
    auto mean = std::accumulate(laps.begin(), laps.end(), 0us) / laps.size();
    ASSERT_TRUE(995us < mean && mean < 1005us);
}

} // namespace test