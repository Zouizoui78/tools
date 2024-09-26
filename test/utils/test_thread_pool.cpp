#include "tools/utils/thread_pool.hpp"
#include "gtest/gtest.h"

#include <iostream>

namespace test {

using namespace tools;

TEST(TestThreadPool, test_thread_pool) {
    std::atomic<bool> test = false;

    ThreadPool pool;
    pool.enqueue_tasks({[&test] {
        std::cout << "setting variable" << std::endl;
        test = true;
    }});
    pool.wait();
    std::cout << "done waiting" << std::endl;

    ASSERT_TRUE(test);
}

} // namespace test