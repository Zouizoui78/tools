#include <gtest/gtest.h>
#include "tools/utils/Log.hpp"

namespace test {

using namespace tools::utils;

class TestLog:   public ::testing::Test
{
    protected:
        TestLog() {}

        virtual ~TestLog() {}

        virtual void SetUp() {}

        virtual void TearDown() {}

    public:
};

TEST_F(TestLog, test_name) {
    /* Test code here */
}

} // namespace test