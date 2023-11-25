#include "gtest/gtest.h"

#include "tools/sdl/sdl_audio_device.hpp"

namespace test {

using namespace tools::sdl;

TEST(TestSDLAudioDevice, test_lifetime_management) {
    ASSERT_FALSE(std::is_copy_constructible_v<tools::sdl::SDLAudioDevice>);
    ASSERT_FALSE(std::is_move_constructible_v<tools::sdl::SDLAudioDevice>);
    ASSERT_FALSE(std::is_copy_assignable_v<tools::sdl::SDLAudioDevice>);
    ASSERT_FALSE(std::is_move_assignable_v<tools::sdl::SDLAudioDevice>);
}

} // namespace test