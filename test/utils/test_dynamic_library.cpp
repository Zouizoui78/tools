#include "gtest/gtest.h"
#include "tools/utils/dynamic_library.hpp"
#include <filesystem>

namespace test {

using namespace tools::utils;

class TestDynamicLibrary: public ::testing::Test
{
    protected:
    TestDynamicLibrary() {
    #ifdef WIN32
        std::string dynlib_filename = "libdynlib.dll";
    #else
        std::string dynlib_filename = "libdynlib.so";
    #endif

        dynlib_path =
            (std::filesystem::path(std::getenv("DYNLIB_PATH")) / dynlib_filename)
            .string();
    }

    virtual ~TestDynamicLibrary() {}

    virtual void SetUp() override {}

    virtual void TearDown() override {}

    public:
    std::string dynlib_path;
};

TEST_F(TestDynamicLibrary, test_load_library) {
    DynamicLibrary lib(dynlib_path);
    ASSERT_TRUE(lib.is_loaded());
}

TEST_F(TestDynamicLibrary, test_load_function) {
    DynamicLibrary lib(dynlib_path);
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
    DynamicLibrary lib(dynlib_path);
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