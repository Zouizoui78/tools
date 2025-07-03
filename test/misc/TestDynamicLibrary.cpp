#include "tools/misc/DynamicLibrary.hpp"
#include "gtest/gtest.h"

#include <filesystem>

namespace test {

using namespace tools;

class TestDynamicLibrary : public ::testing::Test {
protected:
    TestDynamicLibrary() {
        dynlib_path = std::getenv("DYNLIB_PATH");
    }

public:
    std::string dynlib_path;
};

TEST_F(TestDynamicLibrary, test_dynlib_load) {
    ASSERT_NO_THROW(DynamicLibrary lib(dynlib_path));
}

TEST_F(TestDynamicLibrary, test_dynlib_get_function) {
    DynamicLibrary lib(dynlib_path);

    auto func = lib.get_function<bool, const std::string &>("func");
    ASSERT_TRUE(func);

    ASSERT_TRUE(func("test"));
    ASSERT_FALSE(func("bonjour"));

    auto add_ptr(lib.get_function<int, int, int>("add"));
    ASSERT_TRUE(add_ptr);
    ASSERT_EQ(add_ptr(12, 56), 68);

    ASSERT_EQ(lib.get_function<void>("not_a_function"), nullptr);
}

TEST_F(TestDynamicLibrary, test_dynlib_call) {
    DynamicLibrary lib(dynlib_path);

    bool ret = false;
    std::string str = "test";

    // We can omit the arguments types in the signature if they are obvious.
    // There are obvious if a declared variable is passed
    // but not e.g. if we pass "test" directly as an string literal instead of
    // an std::string.
    ASSERT_NO_THROW(ret = lib.call<bool>("func", str));
    ASSERT_TRUE(ret);

    // For example in this case we need the whole signature in the template
    // because otherwise at runtime "nonsense" is passed as a char *,
    // causing a segfault since the function expects an std::string.
    ret = lib.call<bool, const std::string &>("func", "nonsense");
    ASSERT_FALSE(ret);

    ASSERT_THROW(lib.call<bool>("not_a_function", "test"),
                 std::bad_function_call);
    ASSERT_EQ(lib.call<int>("add", 4, 5), 9);
}

} // namespace test
