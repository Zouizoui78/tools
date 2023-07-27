#include "gtest/gtest.h"
#include "spdlog/spdlog.h"

#include "utils/Worker.hpp"
#include "utils/Stopwatch.hpp"

namespace test {

using namespace tools::utils;

class TestWorker: public ::testing::Test
{
    protected:
    TestWorker() {
        outputs_path = std::string(std::getenv("TEST_OUTPUTS"));
    }

    virtual ~TestWorker() {}

    virtual void SetUp() {}

    virtual void TearDown() {}

    public:
    std::string outputs_path;
};

TEST_F(TestWorker, test_task) {
    std::atomic<uint8_t> count = 0;
    Worker task([&count]() {
        ++count;
    });

    ASSERT_FALSE(task.is_running());

    task.set_delay_ms(1);
    task.start();
    ASSERT_TRUE(task.is_running());

    while (count < 3);

    task.stop();
    ASSERT_FALSE(task.is_running());

    ASSERT_EQ(count, 3);
}

TEST_F(TestWorker, test_change_task) {
    std::atomic<uint8_t> a = 0;
    std::atomic<uint8_t> b = 0;

    Worker task([&a]() {
        ++a;
        spdlog::debug("Callback called, a = {}", a.load());
    });

    task.set_delay_ms(1);
    task.start();
    ASSERT_TRUE(task.is_running());

    while (a < 5);

    task.set_task([&b]() {
        ++b;
        spdlog::debug("Callback called, b = {}", b.load());
    });

    spdlog::info("New task set.");

    uint8_t backup_a = a;

    while (b < 10);

    task.stop();
    ASSERT_FALSE(task.is_running());

    EXPECT_EQ(a, backup_a);
}

TEST_F(TestWorker, test_empty_callback) {
    std::function<void ()> fun;
    Worker task(fun);

    EXPECT_FALSE(task.is_running());

    spdlog::info("Error is expected here.");
    task.start();
    EXPECT_FALSE(task.is_running());
}

// TEST_F(TestWorker, test_time_accuracy) {
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
//             spdlog::info("{} ms", diff);
//             EXPECT_NEAR(diff, duration, duration_error);
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