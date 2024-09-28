#include "tools/utils/stopwatch.hpp"
#include "gtest/gtest.h"

#include <thread>

namespace test {

using namespace tools;
using namespace std::chrono;

TEST(TestStopwatch, test_get_duration) {
    int sleep_duration_ms = 50;
    int sleep_duration_us = 50000;

    utils::Stopwatch sw;
    std::this_thread::sleep_for(milliseconds(sleep_duration_ms));
    auto count1 = sw.get_duration<milliseconds>();

    sw.reset();
    std::this_thread::sleep_for(microseconds(sleep_duration_us));
    double count2 = sw.get_duration<microseconds>();

    ASSERT_NEAR(count1, sleep_duration_ms, 5);
    ASSERT_NEAR(count2, sleep_duration_us, 5000);
}

TEST(TestStopwatch, test_laps) {
    int sleep_duration_ms = 10;
    int laps_count = 10;

    utils::Stopwatch sw;

    for (int i = 0; i < laps_count; ++i) {
        std::this_thread::sleep_for(milliseconds(sleep_duration_ms));
        sw.lap();
    }

    auto laps = sw.get_laps<milliseconds>();
    for (utils::Stopwatch::Lap& lap : laps) {
        printf("%f\n", lap.duration);
    }

    ASSERT_EQ(laps.size(), 10);
    for (int i = 0; i < laps.size(); ++i) {
        int margin = 10;
        ASSERT_NEAR(laps[i].duration, sleep_duration_ms, 10);
        ASSERT_NEAR(laps[i].global_duration, (i + 1) * sleep_duration_ms, 10);
    }
}

} // namespace test