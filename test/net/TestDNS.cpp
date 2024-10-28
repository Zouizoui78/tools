#include "gtest/gtest.h"

#include "tools/net/dns.hpp"

#include <algorithm>
#include <print>

namespace test {

class TestDNS : public ::testing::Test {
public:
    TestDNS() {
#if defined(_WIN32)
        WSAData wsaData;
        int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (err != 0) {
            /* Tell the user that we could not find a usable */
            /* Winsock DLL.                                  */
            throw std::runtime_error("WSAStartup failed with error: " +
                                     std::to_string(err));
        }
#endif
    }

    ~TestDNS() override {
#if defined(_WIN32)
        WSACleanup();
#endif
    }
};

TEST_F(TestDNS, test_dns_lookup_str) {
    std::string expected("1.1.1.1");
    auto lookup_result = tools::net::dns_lookup_str("one.one.one.one");
    ASSERT_TRUE(std::ranges::contains(lookup_result, expected));
}

} // namespace test