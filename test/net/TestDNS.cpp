#include "gtest/gtest.h"

#include "tools/net/dns.hpp"

#include <algorithm>
#include <print>

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
    for (const auto &addr : *lookup_result) {
        std::println("{}", addr.str());
    }
    ASSERT_TRUE(
        std::ranges::find_if(*lookup_result, [&expected](const IPAddr &addr) {
            return addr.str() == expected;
        }) != lookup_result->end());
}

} // namespace test