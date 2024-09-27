#include "tools/utils/thread_pool.hpp"
#include "gtest/gtest.h"

#include <iostream>

namespace test {

using namespace tools;

TEST(TestThreadPool, test_thread_pool) {
    bool test = false;

    ThreadPool pool;
    pool.enqueue([&test] {
        std::cout << "setting variable" << std::endl;
        test = true;
    });
    pool.wait();
    std::cout << "done waiting" << std::endl;

    ASSERT_TRUE(test);
}

TEST(TestThreadPool, test_returning_task) {
    int counter = 0;

    ThreadPool pool;
    auto future = pool.enqueue([counter] { return counter + 1; });

    int new_counter = future.get();
    ASSERT_EQ(new_counter, counter + 1);
}

} // namespace test