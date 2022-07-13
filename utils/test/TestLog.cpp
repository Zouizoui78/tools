#include <gtest/gtest.h>
#include "tools/utils/Log.hpp"

namespace test {

using namespace tools::utils;

class TestLog:   public ::testing::Test
{
    protected:
    TestLog() {
        outputs_path = std::string(std::getenv("TEST_OUTPUTS"));
    }

    virtual ~TestLog() {}

    virtual void SetUp() {}

    virtual void TearDown() {}

    public:
    std::string outputs_path;
};

TEST_F(TestLog, test_name) {
    /* Test code here */
}

} // namespace test