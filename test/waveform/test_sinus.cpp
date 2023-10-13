#include "tools/waveform/sinus.hpp"
#include "gtest/gtest.h"

#include <numbers>

namespace test {

class TestSinus : public ::testing::Test {
public:
    tools::waveform::Sinus sinus;
};

double angular_freq(double freq) { return 2.0 * std::numbers::pi * freq; }

TEST_F(TestSinus, test_angular_freq_update) {
    ASSERT_DOUBLE_EQ(sinus.get_angular_freq(),
                     angular_freq(sinus.get_frequency()));
    sinus.set_frequency(1000);
    ASSERT_DOUBLE_EQ(sinus.get_angular_freq(), angular_freq(1000));
}

} // namespace test
