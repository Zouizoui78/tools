#include "tools/utils/str.hpp"
#include "gtest/gtest.h"

namespace test {

using namespace tools::str;

TEST(TestStr, test_stoi) {
    ASSERT_EQ(stoi("24"), 24);
    ASSERT_EQ(stoi("24.3"), 24);
    ASSERT_EQ(stoi("   24"), 24);
    ASSERT_EQ(stoi("24aaa"), 24);
    ASSERT_EQ(stoi("   24.3aaa"), 24);

    ASSERT_EQ(stoi("aaa"), std::nullopt); // invalid_argument
    ASSERT_EQ(stoi("11111111111111111111111111111"),
              std::nullopt); // out_of_range
}

TEST(TestStr, test_stod) {
    ASSERT_DOUBLE_EQ(stod("24").value(), 24);
    ASSERT_DOUBLE_EQ(stod("24.3").value(), 24.3);
    ASSERT_DOUBLE_EQ(stod("   24").value(), 24);
    ASSERT_DOUBLE_EQ(stod("24aaa").value(), 24);
    ASSERT_DOUBLE_EQ(stod("   24.3aaa").value(), 24.3);

    ASSERT_FALSE(stod("aaa"));   // invalid_argument
    ASSERT_FALSE(stod("1e999")); // out_of_range
}

} // namespace test