#include "tools/utils/stopwatch.hpp"
#include "gtest/gtest.h"

#include <thread>

namespace test {

using namespace tools::utils;
using namespace std::chrono;

TEST(TestStopwatch, test_stopwatch) {
    Stopwatch s("test");
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

} // namespace test