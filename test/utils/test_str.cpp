#include "tools/utils/str.hpp"
#include "gtest/gtest.h"

namespace test {

TEST(TestStr, test_stoi) {
    ASSERT_EQ(tools::str::stoi("24"), 24);
    ASSERT_EQ(tools::str::stoi("24.3"), 24);
    ASSERT_EQ(tools::str::stoi("   24"), 24);
    ASSERT_EQ(tools::str::stoi("24aaa"), 24);
    ASSERT_EQ(tools::str::stoi("   24.3aaa"), 24);

    ASSERT_EQ(tools::str::stoi("aaa"), std::nullopt); // invalid_argument
    ASSERT_EQ(tools::str::stoi("11111111111111111111111111111"),
              std::nullopt); // out_of_range
}

TEST(TestStr, test_stod) {
    ASSERT_DOUBLE_EQ(tools::str::stod("24").value(), 24);
    ASSERT_DOUBLE_EQ(tools::str::stod("24.3").value(), 24.3);
    ASSERT_DOUBLE_EQ(tools::str::stod("   24").value(), 24);
    ASSERT_DOUBLE_EQ(tools::str::stod("24aaa").value(), 24);
    ASSERT_DOUBLE_EQ(tools::str::stod("   24.3aaa").value(), 24.3);

    ASSERT_FALSE(tools::str::stod("aaa"));   // invalid_argument
    ASSERT_FALSE(tools::str::stod("1e999")); // out_of_range
}

} // namespace test