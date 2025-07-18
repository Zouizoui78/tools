#include "gtest/gtest.h"

#include "tools/net/dns.hpp"

#include <algorithm>

namespace test {

using namespace tools::net;

class TestDNS : public ::testing::Test {
public:
    TestDNS() {
#if defined(_WIN32)
        WSAData wsaData;
        int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (err != 0) {
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

TEST_F(TestDNS, test_lookup) {
    std::string expected("1.1.1.1");
    auto lookup_result{tools::net::dns::lookup("one.one.one.one")};
    ASSERT_TRUE(lookup_result);
    ASSERT_NE(std::ranges::find_if(*lookup_result,
                                   [&expected](const IPAddr &addr) {
                                       return addr.to_string() == expected;
                                   }),
              lookup_result->end());
}

TEST_F(TestDNS, test_lookup_failure) {
    ASSERT_FALSE(tools::net::dns::lookup("azertyuiop.test"));
}

TEST_F(TestDNS, test_rlookup) {
    IPAddr addr("1.1.1.1");
    std::string expected("one.one.one.one");
    auto rlookup_result{tools::net::dns::rlookup(addr)};
    ASSERT_TRUE(rlookup_result);
    ASSERT_EQ(*rlookup_result, expected);
}
TEST_F(TestDNS, test_rlookup_failure) {
    IPAddr addr("255.255.255.255");
    ASSERT_FALSE(tools::net::dns::rlookup(addr));
}

} // namespace test
