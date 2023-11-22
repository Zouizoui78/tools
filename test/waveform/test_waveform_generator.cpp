#include "gtest/gtest.h"

#include "tools/waveform/constants.hpp"
#include "tools/waveform/waveform_generator.hpp"
#include "tools/waveform/waveforms.hpp"

#include <cmath>
#include <numbers>

namespace test {

std::vector<double> synthesize_sinus(int n_samples) {
    double time = 0;
    double volume = tools::waveform::constants::volume_mult;
    std::vector<double> ret;
    for (int i = 0; i < n_samples; i++) {
        time =
            static_cast<double>(i) / tools::waveform::constants::sampling_rate;
        double sample1 = volume * sin(2.0 * std::numbers::pi * 440 * time);
        double sample2 =
            volume * 0.3 * sin(2.0 * std::numbers::pi * 440 * 3 * time);
        ret.push_back(sample1 + sample2);
    }
    return ret;
}

class TestWaveformGenerator : public ::testing::Test {
public:
    tools::waveform::WaveformGenerator generator;
};

TEST_F(TestWaveformGenerator, test_add_waves) {
    tools::waveform::Sinus sinus;
    tools::waveform::Sinus sinus2;
    ASSERT_TRUE(generator.add_waveform(&sinus));
    ASSERT_FALSE(generator.add_waveform(&sinus));
    ASSERT_TRUE(generator.add_waveform(&sinus2));
    ASSERT_FALSE(generator.add_waveform(&sinus2));
}

TEST_F(TestWaveformGenerator, test_remove_waves) {
    tools::waveform::Sinus sinus;
    tools::waveform::Sinus sinus2;
    generator.add_waveform(&sinus);
    generator.add_waveform(&sinus2);

    ASSERT_TRUE(generator.remove_waveform(&sinus));
    ASSERT_FALSE(generator.remove_waveform(&sinus));

    ASSERT_TRUE(generator.remove_waveform(&sinus2));
    ASSERT_FALSE(generator.remove_waveform(&sinus2));
}

TEST_F(TestWaveformGenerator, test_sample_generation) {
    int n_samples = 1000;
    auto sinus_samples = synthesize_sinus(n_samples);

    tools::waveform::Sinus sinus;
    generator.add_waveform(&sinus);

    tools::waveform::Sinus sinus2;
    sinus2.set_frequency(440 * 3);
    sinus2.set_volume(0.3);
    generator.add_waveform(&sinus2);

    auto generator_samples = generator.generate_n_samples(n_samples);
    ASSERT_EQ(generator_samples.size(), n_samples);
}

} // namespace test
