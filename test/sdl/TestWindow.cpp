#include "gtest/gtest.h"
#include "spdlog/spdlog.h"

#include "tools/sdl/Window.hpp"
#include "tools/utils/Stopwatch.hpp"
#include "tools/utils/Worker.hpp"

namespace test {

using namespace tools::sdl;

class TestWindow: public ::testing::Test
{
    protected:
    TestWindow() {
        outputs_path = std::getenv("TEST_OUTPUTS");
    }

    virtual ~TestWindow() {}

    virtual void SetUp() {}

    virtual void TearDown() {}

    public:
    std::string outputs_path;
};

// TEST_F(TestWindow, test_window) {
//     Window r;

//     SDL_Rect rect { 100, 100, 100, 100 };

//     SDL_Event event;
//     tools::utils::Worker w([&]() {
//         while (SDL_PollEvent(&event)) {
//             if (event.type == SDL_QUIT)
//                 w.stop();
//         }

//         r.set_draw_color(0, 0, 0);
//         r.clear();
//         r.set_draw_color(255, 0, 0);
//         r.draw_rectangle(&rect, true);
//         r.refresh();
//     }, false);

//     w.set_frequency(144);

//     std::cout << "Do you see a red square in a black window ? (y/n)" << std::endl;
//     std::thread t([&]() {
//         char c;
//         std::cin >> c;
//         EXPECT_EQ(c, 'y');
//         w.stop();
//     });

//     w.start();
//     t.join();
// }

// TEST_F(TestWindow, test_multiple_windows) {
//     Window r("test"), r2("test2");

//     r.set_position(SDL_WINDOWPOS_CENTERED, 300);
//     r2.set_position(SDL_WINDOWPOS_CENTERED, 700);

//     r.set_draw_color(255, 0, 0);
//     r2.set_draw_color(0, 255, 0);

//     SDL_Rect rect { 100, 100, 100, 100 };

//     r.draw_rectangle(&rect, true);
//     r2.draw_rectangle(&rect, true);

//     r.refresh();
//     r2.refresh();

//     SDL_Event event;
//     tools::utils::Worker w([&]() {
//         while (SDL_PollEvent(&event)) {
//             if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
//                 w.stop();
//             }
//         }
//     }, false);

//     w.set_frequency(10);

//     std::cout << "Do you see a red square in a black window and a green square in a second window ? (y/n)" << std::endl;
//     std::thread t([&]() {
//         char c;
//         std::cin >> c;
//         EXPECT_EQ(c, 'y');
//         w.stop();
//     });

//     w.start();
//     t.join();
// }

} // namespace test