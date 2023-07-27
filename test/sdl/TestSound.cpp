#include "gtest/gtest.h"
#include "spdlog/spdlog.h"

#include "sdl/Sound.hpp"
#include "utils/Stopwatch.hpp"

namespace test {

using namespace tools::sdl;
using namespace std::literals;

class TestSound:   public ::testing::Test
{
    protected:
    TestSound() {
        outputs_path = std::getenv("TEST_OUTPUTS");
    }

    virtual ~TestSound() {}

    virtual void SetUp() {}

    virtual void TearDown() {}

    public:
    std::string outputs_path;
};

TEST_F(TestSound, test_init) {
    Sinus sin;
    EXPECT_EQ(sin.get_volume(), 1);
    EXPECT_EQ(sin.get_frequency(), 440);
    EXPECT_EQ(sin.get_period(), 1.0/440);

    Square square;
    EXPECT_EQ(square.get_duty_cycle(), 0.5);
}

TEST_F(TestSound, test_sinus) {
    SoundPlayer player;
    ASSERT_TRUE(player.is_initialized());
    player.play();

    std::vector<Sinus *> sounds;
    player.play();
    for (int i = 0 ; i < 6 ; i++) {
        // Adding harmonics 1 to 10 to form a square signal from sinuses.
        Sinus *sin = new Sinus;
        sin->set_frequency(440 * (i * 2 + 1));
        sin->set_volume(1.0 / (i * 2 + 1));
        spdlog::info(440 * (i * 2 + 1));
        spdlog::info(1.0 / (i * 2 + 1));
        player.add_sound(sin);
        sounds.push_back(sin);
        std::this_thread::sleep_for(1s);
    }

    player.pause();
    for (auto sound : sounds) {
        ASSERT_TRUE(player.remove_sound(sound));
        delete sound;
    }
}

TEST_F(TestSound, test_square) {
    SoundPlayer player;
    ASSERT_TRUE(player.is_initialized());

    Square square;
    square.set_frequency(261.63);
    player.add_sound(&square);

    player.play();

    for (uint8_t i = 0 ; i < 4 ; i++) {
        if (i == 0)
            square.set_duty_cycle(0.125);
        else
            square.set_duty_cycle(0.125 * i * 2);
        spdlog::info("Duty cycle = {}", square.get_duty_cycle());
        std::this_thread::sleep_for(1.5s);
    }
}

TEST_F(TestSound, test_duty_cycle) {
    SoundPlayer player;
    Square square;
    square.set_duty_cycle(0.25);
    player.add_sound(&square);
    player.play();
    std::this_thread::sleep_for(1s);
    square.set_duty_cycle(0.75);
    std::this_thread::sleep_for(1s);
}

TEST_F(TestSound, test_setting_changes) {
    SoundPlayer player;
    ASSERT_TRUE(player.is_initialized());

    double la = 440;
    double si = 494;

    Square sound;
    player.add_sound(&sound);
    player.play();
    for (int i = 0 ; i < 5 ; i++) {
        if (sound.get_frequency() == la)
            sound.set_frequency(si);
        else
            sound.set_frequency(la);
        std::this_thread::sleep_for(400ms);
    }
    for (int i = 0 ; i < 4 ; i++) {
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
