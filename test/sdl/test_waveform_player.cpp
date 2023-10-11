#include "gtest/gtest.h"

#include "tools/sdl/waveform_player.hpp"
#include "tools/utils/stopwatch.hpp"
#include "tools/waveform/awaveform.hpp"
#include "tools/waveform/waveforms.hpp"

namespace test {

using namespace std::literals;
using namespace tools::sdl;
using namespace tools::waveform;

class TestWaveformPlayer: public ::testing::Test {
public:
    std::shared_ptr<WaveformGenerator> generator;
    tools::sdl::WaveformPlayer player;

    TestWaveformPlayer() :
        generator(std::make_shared<WaveformGenerator>()),
        player(generator)
    {}
};

TEST_F(TestWaveformPlayer, test_is_playing) {
    ASSERT_FALSE(player.is_playing());
    player.play();
    ASSERT_TRUE(player.is_playing());
}

TEST_F(TestWaveformPlayer, test_sinus) {
    ASSERT_TRUE(player.is_initialized());
    player.play();

    std::vector<std::shared_ptr<AWaveform>> sounds;
    for (int i = 0 ; i < 6 ; i++) {
        auto sin = std::make_shared<Sinus>();
        sin->set_frequency(440 * (i * 2 + 1));
        sin->set_volume(1.0 / (i * 2 + 1));
        generator->add_waveform(sin);
        sounds.push_back(sin);
        std::this_thread::sleep_for(1s);
    }
}

TEST_F(TestWaveformPlayer, test_square) {
    ASSERT_TRUE(player.is_initialized());

    auto square = std::make_shared<Square>();
    square->set_frequency(261.63);
    generator->add_waveform(square);

    player.play();

    for (int i = 0 ; i < 4 ; i++) {
        if (i == 0)
            square->set_duty_cycle(0.125);
        else
            square->set_duty_cycle(0.125 * i * 2);
        std::this_thread::sleep_for(1.5s);
    }
}

TEST_F(TestWaveformPlayer, test_setting_changes) {
    ASSERT_TRUE(player.is_initialized());

    double la = 440;
    double si = 494;

    auto sound = std::make_shared<Square>();
    generator->add_waveform(sound);
    player.play();
    for (int i = 0 ; i < 5 ; i++) {
        if (sound->get_frequency() == la)
            sound->set_frequency(si);
        else
            sound->set_frequency(la);
        std::this_thread::sleep_for(400ms);
    }
    for (int i = 0 ; i < 4 ; i++) {
        if (i == 0)
            sound->set_duty_cycle(0.125);
        else if (i == 1)
            sound->set_duty_cycle(0.25);
        else if (i == 2)
            sound->set_duty_cycle(0.5);
        else {
            sound->set_duty_cycle(0.75);
        }
        std::this_thread::sleep_for(400ms);
    }
}

} // namespace test
