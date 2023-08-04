#include "gtest/gtest.h"
#include "waveform/Square.hpp"

namespace test {

class TestSquare : public ::testing::Test {
public:
    tools::waveform::Square square;
};

TEST_F(TestSquare, test_set_duty_cycle) {
    ASSERT_DOUBLE_EQ(square.get_duty_cycle(), 0.5);

    square.set_duty_cycle(0.001);
    ASSERT_DOUBLE_EQ(square.get_duty_cycle(), 0.01);

    square.set_duty_cycle(1);
    ASSERT_DOUBLE_EQ(square.get_duty_cycle(), 0.99);
}

} // namespace test
