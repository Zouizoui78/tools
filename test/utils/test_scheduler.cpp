#include "tools/utils/scheduler.hpp"
#include "gtest/gtest.h"

namespace test {

using namespace tools::utils;
using namespace std::literals;

TEST(TestScheduler, test_add_task) {
    Scheduler s;

    Task task_a;
    task_a.name = "task_a";
    task_a.task = [&]() { return true; };

    Task task_b;
    task_b.name = "task_b";
    task_b.task = [&]() { return true; };

    Task task_c;
    task_c.task = [&]() { return true; };

    Task task_d;
    task_d.name = "task_b";

    ASSERT_TRUE(s.add_task(task_a));
    ASSERT_TRUE(s.add_task(task_b));
    ASSERT_FALSE(s.add_task(task_c));
    ASSERT_FALSE(s.add_task(task_d));
    ASSERT_FALSE(s.add_task(Task()));
}

TEST(TestScheduler, test_scheduling) {
    Scheduler s;
    // Needed on windows
    s.set_high_precision(true);

    uint32_t count_a = 0;
    Task task_a;
    task_a.name = "task_a";
    task_a.task = [&]() {
        ++count_a;
        if (count_a == 10)
            s.stop();
        return true;
    };
    task_a.delay_ns = 10ms;

    uint32_t count_b = 0;
    Task task_b;
    task_b.name = "task_b";
    task_b.task = [&]() {
        ++count_b;
        return true;
    };
    task_b.delay_ns = 1ms;

    ASSERT_TRUE(s.add_task(task_a));
    ASSERT_TRUE(s.add_task(task_b));
    ASSERT_FALSE(s.add_task(Task()));

    s.start();

    ASSERT_EQ(count_a, 10);
    ASSERT_GE(count_b, 95);
}

} // namespace test