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

TEST(TestThreadPool, test_multiple_tasks) {
    std::atomic<int> counter = 0;

    std::vector<ThreadPool::Task> tasks(
        {3, {[&counter] {
             std::cout << "Incrementing counter from thread "
                       << std::this_thread::get_id() << std::endl;
             ++counter;
         }}});

    ThreadPool pool;
    pool.enqueue_tasks(tasks);
    pool.wait();

    ASSERT_EQ(counter, tasks.size());
}

} // namespace test