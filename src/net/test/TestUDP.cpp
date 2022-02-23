#include <gtest/gtest.h>
#include "tools/net/UDP.hpp"

namespace test {

using namespace tools::net;

namespace udp_test {
    static auto logger = tools::utils::new_logger("UDP test");
}

using namespace udp_test;

class TestUDP:   public ::testing::Test
{
    protected:
        TestUDP() {}

        virtual ~TestUDP() {}

        virtual void SetUp() {}

        virtual void TearDown() {}

    public:
};

TEST_F(TestUDP, test_udp_listen) {
    UDP *udp = new UDP(3000);
    ASSERT_TRUE(udp->is_ok());

    udp->start_listen([](uint8_t *data) {
        logger->info("First received byte is \"{}\".", (int)data[0]);
    });

    usleep(5e6);
    delete udp;
}

} // namespace test