#include "tools/misc/str.hpp"
#include "gtest/gtest.h"

namespace test {

using namespace tools::str;

TEST(TestStr, test_parse_int_success) {
    ASSERT_EQ(parse_int("24"), 24);
    ASSERT_EQ(parse_int("24.3"), 24);
    ASSERT_EQ(parse_int("   24"), 24);
    ASSERT_EQ(parse_int("24aaa"), 24);
    ASSERT_EQ(parse_int("   24.3aaa"), 24);
}

TEST(TestStr, test_parse_int_invalid_argument) {
    ASSERT_EQ(parse_int("aaa").error(), ParseError::INVALID_ARGUMENT);
}

TEST(TestStr, test_parse_int_out_of_range) {
    ASSERT_EQ(parse_int("11111111111111111111111111111").error(),
              ParseError::OUT_OF_RANGE);
}
TEST(TestStr, test_parse_double_success) {
    ASSERT_DOUBLE_EQ(parse_double("24").value(), 24);
    ASSERT_DOUBLE_EQ(parse_double("24.3").value(), 24.3);
    ASSERT_DOUBLE_EQ(parse_double("   24").value(), 24);
    ASSERT_DOUBLE_EQ(parse_double("24aaa").value(), 24);
    ASSERT_DOUBLE_EQ(parse_double("   24.3aaa").value(), 24.3);
}

TEST(TestStr, test_parse_double_invalid_argument) {
    ASSERT_EQ(parse_double("aaa").error(), ParseError::INVALID_ARGUMENT);
}

TEST(TestStr, test_parse_double_out_of_range) {
    ASSERT_EQ(parse_double("1e999").error(), ParseError::OUT_OF_RANGE);
}

} // namespace test
