#include <gtest/gtest.h>
#include "tools/utils/Stopwatch.hpp"

namespace test {

using namespace tools::utils;
using namespace std::chrono;

class TestStopwatch:   public ::testing::Test
{
    protected:
        TestStopwatch() {}

        virtual ~TestStopwatch() {}

        virtual void SetUp() {}

        virtual void TearDown() {}

    public:
};

TEST_F(TestStopwatch, test_stopwatch) {
    Stopwatch s("test");
    usleep(1e3);
}

} // namespace test