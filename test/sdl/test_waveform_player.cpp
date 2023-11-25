#include "gtest/gtest.h"

#include "tools/sdl/waveform_player.hpp"
#include "tools/waveform/waveforms.hpp"

namespace test {

using namespace std::literals;
using namespace tools::sdl;
using namespace tools::waveform;

class TestWaveformPlayer : public ::testing::Test {
public:
    WaveformPlayer player;
};

TEST_F(TestWaveformPlayer, test_lifetime_management) {
    ASSERT_FALSE(std::is_copy_constructible_v<WaveformPlayer>);
    ASSERT_FALSE(std::is_copy_assignable_v<WaveformPlayer>);
    ASSERT_FALSE(std::is_move_constructible_v<WaveformPlayer>);
    ASSERT_FALSE(std::is_move_assignable_v<WaveformPlayer>);
}

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
        auto sin = sounds.back().get();
        sin->set_frequency(440 * (i * 2 + 1));
        sin->set_volume(1.0 / (i * 2 + 1));
        player.add_waveform(sin);
        std::this_thread::sleep_for(1s);
    }
}

TEST_F(TestWaveformPlayer, test_square) {
    Square square;
    square.set_frequency(261.63);
    player.add_waveform(&square);

    player.play();

    square.set_duty_cycle(0.125);
    std::this_thread::sleep_for(1.5s);

    square.set_duty_cycle(0.25);
    std::this_thread::sleep_for(1.5s);

    square.set_duty_cycle(0.5);
    std::this_thread::sleep_for(1.5s);

    square.set_duty_cycle(0.75);
    std::this_thread::sleep_for(1.5s);
}

TEST_F(TestWaveformPlayer, test_setting_changes) {
    double la = 440;
    double si = 494;

    Square sound;
    sound.set_frequency(la);
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
