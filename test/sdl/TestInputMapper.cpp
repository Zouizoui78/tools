#include "gtest/gtest.h"
#include "spdlog/spdlog.h"

#include "tools/sdl/InputMapper.hpp"
#include "tools/utils/fs.hpp"

namespace test {

using namespace tools::sdl;

class TestInputMapper: public ::testing::Test
{
    protected:
    TestInputMapper() {
        outputs_path = std::getenv("TEST_OUTPUTS");
    }

    virtual ~TestInputMapper() {}

    virtual void SetUp() {}

    virtual void TearDown() {}

    public:
    std::string outputs_path;
};

TEST_F(TestInputMapper, test_add_remove_mapping) {
    spdlog::info("Errors are expected in this test.");
    InputMapper mapper;
    ASSERT_TRUE(mapper.set_mapping("a", 17));
    ASSERT_TRUE(mapper.set_mapping("a", 45));
    mapper.set_mapping(SDL_KeyCode::SDLK_a, 45);
    ASSERT_EQ(mapper.map_key(SDL_KeyCode::SDLK_a), 45);
    ASSERT_EQ(mapper.map_key("a"), 45);

    ASSERT_FALSE(mapper.set_mapping("nonsense", 1));

    ASSERT_TRUE(mapper.set_mapping("b", 32));
    ASSERT_TRUE(mapper.remove_mapping("a"));
    ASSERT_EQ(mapper.map_key("a"), -1);
    ASSERT_TRUE(mapper.remove_mapping("a"));
}

} // namespace test