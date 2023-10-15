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
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(1ms);
        s.lap();
    }

    auto laps = s.get_laps<microseconds>();
    auto mean = std::accumulate(laps.begin(), laps.end(), 0us) / laps.size();
    ASSERT_TRUE(1ms < mean && mean < 1100us);
}

} // namespace test