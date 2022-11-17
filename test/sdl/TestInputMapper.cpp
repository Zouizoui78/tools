#include <gtest/gtest.h>
#include "tools/sdl/InputMapper.hpp"

#include "tools/utils/Files.hpp"
#include "tools/utils/Log.hpp"

namespace test {

using namespace tools::sdl;
static auto logger = tools::utils::new_logger("TestInputMapper");

class TestInputMapper:   public ::testing::Test
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
    logger->info("Errors are expected in this test.");
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

TEST_F(TestInputMapper, test_json_map) {
    InputMapper mapper;
    std::string path = "test_resources/InputMapper/test_map.json";

    ASSERT_TRUE(mapper.load_map(path));
    ASSERT_EQ(mapper.map_key("a"), 1);
    ASSERT_EQ(mapper.map_key("z"), 3);
    ASSERT_EQ(mapper.map_key("e"), 67);
    ASSERT_EQ(mapper.map_key("r"), 53);
    ASSERT_EQ(mapper.map_key("t"), 4);
    ASSERT_EQ(mapper.map_key("y"), 98);
}

} // namespace test