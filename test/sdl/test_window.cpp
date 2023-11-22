#include "gtest/gtest.h"

#include "tools/sdl/window.hpp"
#include "tools/utils/stopwatch.hpp"

namespace test {

using namespace tools::sdl;

TEST(TestWindow, test_window) {
    Window r;

    SDL_Rect rect{100, 100, 100, 100};
    r.set_draw_color(0, 0, 0);
    r.clear();
    r.set_draw_color(255, 0, 0);
    r.draw_rectangle(&rect, true);
    r.refresh();

    std::cout << "Do you see a red square in a black window ? (y/n)"
              << std::endl;
    char c;
    std::cin >> c;
    EXPECT_EQ(c, 'y');
}

TEST(TestWindow, test_multiple_windows) {
    Window r("test"), r2("test2");

    r.set_position(SDL_WINDOWPOS_CENTERED, 300);
    r2.set_position(SDL_WINDOWPOS_CENTERED, 700);

    r.set_draw_color(255, 0, 0);
    r2.set_draw_color(0, 255, 0);

    SDL_Rect rect{100, 100, 100, 100};

    r.draw_rectangle(&rect, true);
    r2.draw_rectangle(&rect, true);

    r.refresh();
    r2.refresh();

    std::cout << "Do you see a red square in a black window and a green square "
                 "in a second window ? (y / n) "
              << std::endl;

    char c;
    std::cin >> c;
    EXPECT_EQ(c, 'y');
}

} // namespace test