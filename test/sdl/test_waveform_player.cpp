#include "gtest/gtest.h"

#include "tools/sdl/waveform_player.hpp"
#include "tools/waveform/waveforms.hpp"

namespace test {

using namespace std::literals;
using namespace tools::sdl;
using namespace tools::waveform;

class TestWaveformPlayer : public ::testing::Test {
public:
    tools::sdl::WaveformPlayer player;

    TestWaveformPlayer() {}
};

TEST_F(TestWaveformPlayer, test_is_paused) {
    ASSERT_TRUE(player.is_paused());
    player.play();
    ASSERT_FALSE(player.is_paused());
}

TEST_F(TestWaveformPlayer, test_sinus) {
    player.play();

    std::vector<std::unique_ptr<WaveformBase>> sounds;
    for (int i = 0; i < 6; i++) {
        sounds.emplace_back(std::make_unique<Sinus>());
        auto& sin = *(sounds.back());
        sin.set_frequency(440 * (i * 2 + 1));
        sin.set_volume(1.0 / (i * 2 + 1));
        player.add_waveform(&sin);
        std::this_thread::sleep_for(1s);
    }
}

TEST_F(TestWaveformPlayer, test_square) {
    Square square;
    square.set_frequency(261.63);
    player.add_waveform(&square);

    player.play();

    for (int i = 0; i < 4; i++) {
        if (i == 0)
            square.set_duty_cycle(0.125);
        else
            square.set_duty_cycle(0.125 * i * 2);
        std::this_thread::sleep_for(1.5s);
    }
}

TEST_F(TestWaveformPlayer, test_setting_changes) {
    double la = 440;
    double si = 494;

    Square sound;
    player.add_waveform(&sound);
    player.play();
    for (int i = 0; i < 5; i++) {
        if (sound.get_frequency() == la)
            sound.set_frequency(si);
        else
            sound.set_frequency(la);
        std::this_thread::sleep_for(400ms);
    }
    for (int i = 0; i < 4; i++) {
        if (i == 0)
            sound.set_duty_cycle(0.125);
        else if (i == 1)
            sound.set_duty_cycle(0.25);
        else if (i == 2)
            sound.set_duty_cycle(0.5);
        else {
            sound.set_duty_cycle(0.75);
        }
        std::this_thread::sleep_for(400ms);
    }
}

} // namespace test
