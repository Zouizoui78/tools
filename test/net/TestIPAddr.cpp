#include "gtest/gtest.h"

#include "tools/net/IPAddr.hpp"

#include <algorithm>

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

    std::string actual_4(IPAddr(expected_4).str());
    std::string actual_6(IPAddr(expected_6).str());

    ASSERT_EQ(actual_4, expected_4);
    ASSERT_EQ(actual_6, expected_6);
    ASSERT_EQ(actual_4.size(), expected_4.size());
    ASSERT_EQ(actual_6.size(), expected_6.size());
}

TEST_F(TestIPAddr, test_ctor_invalid_str) {
    ASSERT_THROW(IPAddr addr{"abcd"}, std::invalid_argument);
}

TEST_F(TestIPAddr, test_ctor_addrinfo_nullptr) {
    ASSERT_THROW(IPAddr{nullptr}, std::invalid_argument);
}

TEST_F(TestIPAddr, test_ctor_addrinfo_invalid_addr_family) {
    addrinfo addr_info;
    addr_info.ai_family = AF_UNIX;
    ASSERT_THROW(IPAddr{&addr_info}, std::invalid_argument);
}

} // namespace test