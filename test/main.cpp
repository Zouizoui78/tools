#include "gtest/gtest.h"

#include <filesystem>

int main(int argc, char **argv)
{
    std::string outputs_path = std::getenv("TEST_OUTPUTS");
    std::filesystem::create_directories(outputs_path);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}