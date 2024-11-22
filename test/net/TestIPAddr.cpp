#include "gtest/gtest.h"

#include "tools/net/IPAddr.hpp"

#include <algorithm>
#include <print>

namespace test {

using namespace tools::net;

class TestIPAddr : public ::testing::Test {
public:
    TestIPAddr() {
#if defined(_WIN32)
        WSAData wsaData;
        int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (err != 0) {
            throw std::runtime_error("WSAStartup failed with error: " +
                                     std::to_string(err));
        }
#endif
    }

    ~TestIPAddr() override {
#if defined(_WIN32)
        WSACleanup();
#endif
    }
};

TEST_F(TestIPAddr, test_ctor_str) {
    std::string expected_4("127.0.0.1");
    std::string expected_6("::1");
    IPAddr addr4(expected_4);
    ASSERT_EQ(addr4.str(), expected_4);
    IPAddr addr6(expected_6);
    ASSERT_EQ(addr6.str(), expected_6);
}

} // namespace test