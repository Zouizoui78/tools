#include "tools/misc/ThreadPool.hpp"
#include "gtest/gtest.h"

#include "tools/misc/Stopwatch.hpp"

namespace test {

using namespace tools;

TEST(TestThreadPool, test_thread_pool) {
    std::atomic<bool> test = false;

    ThreadPool pool;
    pool.enqueue([&test] {
        test = true;
    });

    pool.wait();
    ASSERT_TRUE(test);
}

TEST(TestThreadPool, test_returning_task) {
    std::string test_str("test");

    ThreadPool pool;
    auto future = pool.enqueue([test_str] {
        return test_str;
    });

    ASSERT_EQ(future.get(), test_str);
}

TEST(TestThreadPool, test_spam) {
    std::atomic<int> counter = 0;
    int job_count = 1000;

    ThreadPool pool;

    for (int i = 0; i < job_count; ++i) {
        pool.enqueue([&counter] {
            ++counter;
        });
    }

    pool.wait();

    ASSERT_EQ(counter, job_count);
}

TEST(TestThreadPool, test_start_stop) {
    using namespace std::chrono;

    std::atomic<int> counter = 0;

    auto long_task = [&counter] {
        std::this_thread::sleep_for(milliseconds(10));
        ++counter;
    };

    auto short_task = [&counter] {
        std::this_thread::sleep_for(milliseconds(1));
        ++counter;
    };

    ThreadPool pool(2);
    pool.enqueue(long_task);
    pool.enqueue(long_task);
    pool.enqueue(short_task);
    pool.enqueue(short_task);

    // Ensure the two long tasks have started.
    std::this_thread::sleep_for(milliseconds(1));

    pool.stop();

    ASSERT_EQ(counter, 2);

    pool.start();
    pool.wait();

    ASSERT_EQ(counter, 4);
}

// This test checks that a thread that just finished a task
// properly picks a next one without waiting for a notification
// from another thread.
TEST(TestThreadPool, test_task_picked_when_one_is_done) {
    using namespace std::chrono;

    std::atomic<int> counter = 0;

    // The test duration should roughly equal the duration of this task.
    auto task1 = [&counter] {
        std::this_thread::sleep_for(milliseconds(10));
        ++counter;
    };

    // The thread picking up this task should be done quickly
    // and pick up a second task before task1 is done.
    auto task2 = [&counter] {
        std::this_thread::sleep_for(milliseconds(1));
        ++counter;
    };

    auto task3 = [&counter] {
        std::this_thread::sleep_for(milliseconds(5));
        ++counter;
    };

    ThreadPool pool(2);
    pool.enqueue(task1);
    pool.enqueue(task2);
    pool.enqueue(task3);

    Stopwatch sw;

    pool.start();
    pool.wait();

    auto duration_ms = sw.get_duration<milliseconds>();

    // If tasks are properly picked up by threads who just finished their
    // previous tasks, all tasks should take 10ms and 11ms. If they are
    // not picked up, the task that processes task2 would wait for the
    // notification from the thread processing task1, so the three tasks would
    // take at least 15ms.
    ASSERT_GT(duration_ms, 10);
    ASSERT_LT(duration_ms, 11);
}

} // namespace test
