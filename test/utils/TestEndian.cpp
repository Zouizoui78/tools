#include "tools/utils/endian.hpp"
#include "gtest/gtest.h"

namespace test {

using namespace tools::endian;

TEST(TestEndian, test_endian_swap_bytes) {
    int in = 0x01020304;
    int out = swap(in);
    ASSERT_EQ(out, 0x04030201);

    int16_t in16 = 0xaabb;
    int16_t out16 = swap(in16);
    ASSERT_EQ(out16, static_cast<int16_t>(0xbbaa));

    double d = 1.234;
    double dout = swap(d);
    ASSERT_NE(d, dout);
    dout = swap(dout);
    ASSERT_EQ(d, dout);
}

TEST(TestEndian, test_endian_range) {
    std::vector<int16_t> in{static_cast<int16_t>(0xaabb),
                            static_cast<int16_t>(0xccdd)};
    auto out = swap_range(in);
    ASSERT_EQ(out[0], static_cast<int16_t>(0xbbaa));
    ASSERT_EQ(out[1], static_cast<int16_t>(0xddcc));
}

} // namespace test