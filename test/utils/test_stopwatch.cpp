#include "tools/utils/stopwatch.hpp"
#include "gtest/gtest.h"

#include <thread>

namespace test {

using namespace tools::utils;
using namespace std::chrono;

TEST(TestStopwatch, test_stopwatch) {
    Stopwatch s;
    auto d = s.get_duration<nanoseconds>();
    ASSERT_LE(d, 1us);
    std::cout << s.get_duration<nanoseconds>() << std::endl;
}

} // namespace test