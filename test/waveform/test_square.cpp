#include "tools/waveform/square.hpp"
#include "gtest/gtest.h"

namespace test {

using namespace tools::waveform;

class TestSquare : public ::testing::Test {
public:
    Square square;
};

TEST_F(TestSquare, test_set_duty_cycle) {
    ASSERT_DOUBLE_EQ(square.get_duty_cycle(), 0.5);

    square.set_duty_cycle(0.001);
    ASSERT_DOUBLE_EQ(square.get_duty_cycle(), 0.01);

    square.set_duty_cycle(1);
    ASSERT_DOUBLE_EQ(square.get_duty_cycle(), 0.99);
}

} // namespace test
