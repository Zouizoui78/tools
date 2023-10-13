#include "tools/utils/endian.hpp"
#include "gtest/gtest.h"

namespace test {

TEST(TestEndian, test_endian_swap_bytes) {
    int in = 0x01020304;
    int out = tools::endian::swap_bytes(in);
    ASSERT_EQ(out, 0x04030201);
}

} // namespace test