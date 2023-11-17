#include "gtest/gtest.h"

#include "tools/waveform/constants.hpp"
#include "tools/waveform/waveform_base.hpp"


namespace test {

class Waveform : public tools::waveform::WaveformBase {
    double synthesize(tools::waveform::WaveformTimepoint timepoint) const {
        return timepoint.time;
    }
};

class TestWaveformBase : public ::testing::Test {
public:
    Waveform w;
};

TEST_F(TestWaveformBase, test_ctor) {
    ASSERT_DOUBLE_EQ(w.get_volume(), 1);
    ASSERT_DOUBLE_EQ(w.get_frequency(), 440);
    ASSERT_DOUBLE_EQ(w.get_period(), 1.0 / 440.0);
    ASSERT_DOUBLE_EQ(w.get_samples_in_period(),
                     tools::waveform::constants::sampling_rate / 440);
}

TEST_F(TestWaveformBase, test_set_volume) {
    w.set_volume(0.1);
    ASSERT_DOUBLE_EQ(w.get_volume(), 0.1);
    w.set_volume(5);
    ASSERT_DOUBLE_EQ(w.get_volume(), 1);
}

TEST_F(TestWaveformBase, test_set_frequency) {
    w.set_frequency(1000);
    ASSERT_DOUBLE_EQ(w.get_frequency(), 1000);
    ASSERT_DOUBLE_EQ(w.get_period(), 1.0 / 1000.0);

    // Frequency should not change
    w.set_frequency(-2);
    ASSERT_DOUBLE_EQ(w.get_frequency(), 1000);
}

} // namespace test
