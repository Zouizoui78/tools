#include <gtest/gtest.h>
#include "tools/net/UDP.hpp"
#include "tools/utils/Log.hpp"

#include <arpa/inet.h>

namespace test {

using namespace tools::net;
static auto logger = tools::utils::new_logger("TestUDP");

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
    UDP udp(5000);
    ASSERT_TRUE(udp.is_ok());

    const char *data = "test_data";
    char received[10];

    bool callback_called = false;
    size_t received_bytes = 0;
    udp.start_listen([&](uint8_t *data, size_t size) {
        callback_called = true;
        received_bytes = size;

        strcpy(received, (char *)data);
        logger->info("Received {} bytes : {}", size, received);
    });

    int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockaddr_in addr_in;
    addr_in.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_in.sin_port = htons(5000);
    addr_in.sin_family = AF_INET;
    sockaddr *addr = reinterpret_cast<sockaddr *>(&addr_in);

    int res = sendto(s, data, strlen(data) + 1, 0, addr, sizeof(addr_in));

    usleep(1e3);

    ASSERT_NE(res, -1);
    ASSERT_TRUE(callback_called);

    size_t expected_size = 10;
    ASSERT_EQ(received_bytes, expected_size);
    ASSERT_EQ(strcmp(data, received), 0);
}

} // namespace test