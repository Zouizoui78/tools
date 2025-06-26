#include "tools/misc/str.hpp"
#include "gtest/gtest.h"

namespace test {

using namespace tools::str;

TEST(TestStr, test_stoi) {
    ASSERT_EQ(stoi("24"), 24);
    ASSERT_EQ(stoi("24.3"), 24);
    ASSERT_EQ(stoi("   24"), 24);
    ASSERT_EQ(stoi("24aaa"), 24);
    ASSERT_EQ(stoi("   24.3aaa"), 24);

    ASSERT_EQ(stoi("aaa").error(), ErrorCode::INVALID_ARGUMENT);
    ASSERT_EQ(stoi("11111111111111111111111111111").error(),
              ErrorCode::OUT_OF_RANGE);
}

TEST(TestStr, test_stod) {
    ASSERT_DOUBLE_EQ(stod("24").value(), 24);
    ASSERT_DOUBLE_EQ(stod("24.3").value(), 24.3);
    ASSERT_DOUBLE_EQ(stod("   24").value(), 24);
    ASSERT_DOUBLE_EQ(stod("24aaa").value(), 24);
    ASSERT_DOUBLE_EQ(stod("   24.3aaa").value(), 24.3);

    ASSERT_EQ(stod("aaa").error(), ErrorCode::INVALID_ARGUMENT);
    ASSERT_EQ(stod("1e999").error(), ErrorCode::OUT_OF_RANGE);
}

} // namespace test
