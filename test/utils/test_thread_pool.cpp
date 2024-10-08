#include "tools/utils/thread_pool.hpp"
#include "gtest/gtest.h"

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

} // namespace test
