#include "tools/utils/mutexed.hpp"
#include "gtest/gtest.h"

namespace test {

using namespace tools;

class TestClass {
public:
    std::string say_hi() const {
        return "hi";
    }
};

TEST(TestMutexed, test_exception) {
    Mutexed<int> mutexed;
    ASSERT_THROW(*mutexed, std::runtime_error);
}

TEST(TestMutexed, test_no_exception) {
    Mutexed<int> mutexed;
    std::unique_lock lock(mutexed);
    ASSERT_NO_THROW(*mutexed);
}

TEST(TestMutexed, test_assignment) {
    int test_value = 3;
    Mutexed<int> mutexed;

    ASSERT_THROW(mutexed = test_value, std::runtime_error);

    {
        std::scoped_lock lock(mutexed);
        ASSERT_NO_THROW(mutexed = test_value);
        ASSERT_EQ(*mutexed, test_value);
    }
}

TEST(TestMutexed, test_arrow_operator) {
    Mutexed<TestClass> mutexed;
    std::scoped_lock lock(mutexed);
    ASSERT_EQ(mutexed->say_hi(), "hi");
}

} // namespace test