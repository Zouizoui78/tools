#include <gtest/gtest.h>
#include "tools/sdl/Renderer.hpp"
#include "tools/utils/Log.hpp"

#include "tools/utils/Worker.hpp"
#include "tools/utils/Stopwatch.hpp"

namespace test {

using namespace tools::sdl;
static auto logger = tools::utils::new_logger("TestRenderer");

class TestRenderer:   public ::testing::Test
{
    protected:
    TestRenderer() {
        outputs_path = std::getenv("TEST_OUTPUTS");
    }

    virtual ~TestRenderer() {}

    virtual void SetUp() {}

    virtual void TearDown() {}

    public:
    std::string outputs_path;
};

TEST_F(TestRenderer, test_window) {
    Renderer r;
    r.init();

    SDL_Rect rect { 100, 100, 100, 100 };

    SDL_Event event;
    tools::utils::Worker w([&]() {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                w.stop();
        }

        r.set_draw_color(0, 0, 0);
        r.clear();
        r.set_draw_color(255, 0, 0);
        r.draw_rectangle(&rect, true);
        r.refresh();
    }, false);

    w.set_frequency(144);

    std::cout << "Do you see a red square in a black window ? (y/n)" << std::endl;
    std::thread t([&]() {
        char c;
        std::cin >> c;
        EXPECT_EQ(c, 'y');
        w.stop();
    });

    w.start();
    r.stop();
    t.join();
}

} // namespace test