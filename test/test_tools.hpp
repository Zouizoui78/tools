#ifndef TEST_TOOLS_HPP
#define TEST_TOOLS_HPP

#include <filesystem>

namespace test {

inline std::string get_output_path() {
    std::string output_path = std::getenv("TEST_OUTPUT_DIR");
    if (!std::filesystem::exists(output_path)) {
        std::filesystem::create_directories(output_path);
    }
    return output_path;
}

} // namespace test

#endif // TEST_TOOLS_HPP