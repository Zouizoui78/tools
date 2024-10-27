#include "gtest/gtest.h"

#include "tools/net/dns.hpp"

#include <algorithm>
#include <print>

namespace test {

TEST(TestDNS, test_dns_lookup_str) {
    std::vector<std::string> expected{
        "1.0.0.1", "1.1.1.1", "2606:4700:4700::1001", "2606:4700:4700::1111"};

    auto lookup_result = tools::net::dns_lookup_str("one.one.one.one");

    ASSERT_EQ(lookup_result.size(), 4);

    for (const auto &addr : lookup_result) {
        EXPECT_TRUE(std::ranges::contains(expected, addr));
    }
}

} // namespace test