#include "gtest/gtest.h"

#include "tools/waveform/constants.hpp"
#include "tools/waveform/waveform_generator.hpp"
#include "tools/waveform/waveforms.hpp"

#include <cmath>
#include <numbers>

namespace test {

std::vector<double> synthesize_sinus(int n_samples) {
    double time = 0;
    double volume = tools::waveform::volume_mult;
    std::vector<double> ret;
    for (int i = 0 ; i < n_samples ; i++) {
        time = static_cast<double>(i) / tools::waveform::sampling_rate;
        double sample1 = volume * sin(2.0 * std::numbers::pi * 440 * time);
        double sample2 = volume * 0.3 * sin(2.0 * std::numbers::pi * 440 * 3 * time);
        ret.push_back(sample1 + sample2);
    }
    return ret;
}

class TestWaveformGenerator : public ::testing::Test {
public:
    tools::waveform::WaveformGenerator generator;
};

TEST_F(TestWaveformGenerator, test_add_waves) {
    auto sinus = std::make_shared<tools::waveform::Sinus>();
    auto sinus2 = std::make_shared<tools::waveform::Sinus>();
    ASSERT_TRUE(generator.add_waveform(sinus));
    ASSERT_FALSE(generator.add_waveform(sinus));
    ASSERT_TRUE(generator.add_waveform(sinus2));
    ASSERT_FALSE(generator.add_waveform(sinus2));
}

TEST_F(TestWaveformGenerator, test_remove_waves) {
    auto sinus = std::make_shared<tools::waveform::Sinus>();
    auto sinus2 = std::make_shared<tools::waveform::Sinus>();
    generator.add_waveform(sinus);
    generator.add_waveform(sinus2);

    ASSERT_TRUE(generator.remove_waveform(sinus));
    ASSERT_FALSE(generator.remove_waveform(sinus));

    ASSERT_TRUE(generator.remove_waveform(sinus2));
    ASSERT_FALSE(generator.remove_waveform(sinus2));
}

TEST_F(TestWaveformGenerator, test_sample_generation) {
    int n_samples = 1000;
    auto sinus_samples = synthesize_sinus(n_samples);

    auto sinus = std::make_shared<tools::waveform::Sinus>();
    generator.add_waveform(sinus);

    auto sinus2 = std::make_shared<tools::waveform::Sinus>();
    sinus2->set_frequency(440 * 3);
    sinus2->set_volume(0.3);
    generator.add_waveform(sinus2);

    auto generator_samples = generator.generate_n_samples(n_samples);

    double abs_error = tools::waveform::volume_mult / 1000.0;
    for (int i = 0 ; i < n_samples ; i++) {
        ASSERT_NEAR(sinus_samples[i], generator_samples[i], abs_error);
    }
}

} // namespace test
