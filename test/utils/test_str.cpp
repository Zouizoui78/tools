#include "tools/utils/str.hpp"
#include "gtest/gtest.h"

namespace test {

class TestStr : public ::testing::Test {};

TEST_F(TestStr, test_stoi) {
    EXPECT_EQ(tools::str::stoi("24"), 24);
    EXPECT_EQ(tools::str::stoi("24.3"), 24);
    EXPECT_EQ(tools::str::stoi("   24"), 24);
    EXPECT_EQ(tools::str::stoi("24aaa"), 24);
    EXPECT_EQ(tools::str::stoi("   24.3aaa"), 24);

    EXPECT_EQ(tools::str::stoi("aaa"), std::nullopt); // invalid_argument
    EXPECT_EQ(tools::str::stoi("11111111111111111111111111111"),
              std::nullopt); // out_of_range
}

TEST_F(TestStr, test_stod) {
    EXPECT_DOUBLE_EQ(tools::str::stod("24").value(), 24);
    EXPECT_DOUBLE_EQ(tools::str::stod("24.3").value(), 24.3);
    EXPECT_DOUBLE_EQ(tools::str::stod("   24").value(), 24);
    EXPECT_DOUBLE_EQ(tools::str::stod("24aaa").value(), 24);
    EXPECT_DOUBLE_EQ(tools::str::stod("   24.3aaa").value(), 24.3);

    EXPECT_FALSE(tools::str::stod("aaa"));   // invalid_argument
    EXPECT_FALSE(tools::str::stod("1e999")); // out_of_range
}

} // namespace test