#include <gtest/gtest.h>
#include "tools/utils/DynamicLibrary.hpp"
#include "tools/utils/Log.hpp"

namespace test {

using namespace tools::utils;

static auto logger = new_logger("TestDynamicLibrary");

// The small lib used for testing this
// is compiled in the working directory
// when compiling tests.
#ifdef WINDOWS
static std::string path = "./libshared.dll";
#else
static std::string path = "./libshared.so";
#endif

class TestDynamicLibrary:   public ::testing::Test
{
    protected:
    TestDynamicLibrary() {
        outputs_path = std::string(std::getenv("TEST_OUTPUTS"));
    }

    virtual ~TestDynamicLibrary() {}

    virtual void SetUp() override {}

    virtual void TearDown() override {}

    public:
    std::string outputs_path;
};

TEST_F(TestDynamicLibrary, test_load_library) {
    DynamicLibrary lib(path);
    ASSERT_TRUE(lib.is_loaded());
}

TEST_F(TestDynamicLibrary, test_load_function) {
    DynamicLibrary lib(path);
    ASSERT_TRUE(lib.is_loaded());

    auto func_ptr = lib.get_function<bool, const std::string &>("func");
    ASSERT_NE(func_ptr, nullptr);

    ASSERT_TRUE(func_ptr("test"));
    ASSERT_FALSE(func_ptr("bonjour"));

    auto add_ptr(lib.get_function<int, int, int>("add"));
    ASSERT_NE(add_ptr, nullptr);
    ASSERT_EQ(add_ptr(12, 56), 68);

    ASSERT_EQ(lib.get_function<void>("not_a_function"), nullptr);
}

TEST_F(TestDynamicLibrary, test_call) {
    DynamicLibrary lib(path);
    ASSERT_TRUE(lib.is_loaded());

    bool ret = false;
    std::string str = "test";

    // We can omit the arguments types in the signature if they are obvious.
    // There are obvious if a declared variable is passed
    // but not e.g. if we pass "test" directly as an std::string.
    ASSERT_NO_THROW(ret = lib.call<bool>("func", str));
    ASSERT_TRUE(ret);

    // For example in this case we need the whole signature in the template
    // because otherwise at runtime "nonsense" is passed as a char *,
    // causing a segfault since the function expects an std::string.
    ret = lib.call<bool, const std::string &>("func", "nonsense");
    ASSERT_FALSE(ret);

    ASSERT_THROW(lib.call<bool>("not_a_function", "test"), std::bad_function_call);
    ASSERT_EQ(lib.call<int>("add", 4, 5), 9);
}

} // namespace test