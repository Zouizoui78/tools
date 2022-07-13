#include <gtest/gtest.h>
#include "tools/utils/Stopwatch.hpp"

namespace test {

using namespace tools::utils;
using namespace std::chrono;

class TestStopwatch:   public ::testing::Test
{
    protected:
    TestStopwatch() {
        outputs_path = std::string(std::getenv("TEST_OUTPUTS"));
    }

    virtual ~TestStopwatch() {}

    virtual void SetUp() {}

    virtual void TearDown() {}

    public:
    std::string outputs_path;
};

TEST_F(TestStopwatch, test_stopwatch) {
    Stopwatch s("test");
    usleep(1e3);
}

} // namespace test