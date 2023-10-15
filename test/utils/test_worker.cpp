#include "gtest/gtest.h"

#include "tools/utils/stopwatch.hpp"
#include "tools/utils/worker.hpp"

namespace test {

using namespace tools::utils;

TEST(TestWorker, test_task) {
    std::atomic<uint8_t> count = 0;
    Worker task([&count]() { ++count; });

    ASSERT_FALSE(task.is_running());

    task.set_delay_ms(1);
    task.start();
    ASSERT_TRUE(task.is_running());

    while (count < 3)
        ;

    task.stop();
    ASSERT_FALSE(task.is_running());

    ASSERT_EQ(count, 3);
}

TEST(TestWorker, test_change_task) {
    std::atomic<uint8_t> a = 0;
    std::atomic<uint8_t> b = 0;

    Worker task([&a]() { ++a; });

    task.set_delay_ms(1);
    task.start();
    ASSERT_TRUE(task.is_running());

    while (a < 5)
        ;

    task.set_task([&b]() { ++b; });

    uint8_t backup_a = a;

    while (b < 10)
        ;

    task.stop();
    ASSERT_FALSE(task.is_running());

    ASSERT_EQ(a, backup_a);
}

TEST(TestWorker, test_empty_callback) {
    std::function<void()> fun;
    Worker task(fun);

    ASSERT_FALSE(task.is_running());

    task.start();
    ASSERT_FALSE(task.is_running());
}

// TEST(TestWorker, test_time_accuracy) {
//     int count = 0;
//     Stopwatch s("test");

//     double previous = 0;
//     bool first_done = false;

//     uint32_t frequency = 1000;
//     double duration = 1000.0 / frequency;
//     double duration_error = duration / 100;

//     Worker w([&]() {
//         double d = s.get_duration();
//         double diff = (d - previous) / 1e6;
//         previous = d;

//         // We don't expect on first iteration because
//         // then the diff is ~0.
//         if (!first_done)
//             first_done = true;
//         else {
//             ASSERT_NEAR(diff, duration, duration_error);
//         }

//         ++count;

//         if (count == 5)
//             w.stop();
//     }, false);

//     w.set_frequency(frequency);
//     s.reset();
//     w.start();
// }

} // namespace test