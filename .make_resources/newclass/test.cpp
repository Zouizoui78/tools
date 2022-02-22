#include <gtest/gtest.h>
#include "HEADER"

namespace test {

using namespace PROJECT_NAME::MODULE_NAME;

class TestCLASS_NAME:   public ::testing::Test
{
    protected:
        TestCLASS_NAME() {}

        virtual ~TestCLASS_NAME() {}

        virtual void SetUp() {}

        virtual void TearDown() {}

    public:
};

TEST_F(TestCLASS_NAME, test_name) {
    /* Test code here */
}

} // namespace test