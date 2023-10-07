#include "gtest/gtest.h"

#include "tools/utils/file.hpp"
#include "../TestTools.hpp"

namespace test {

TEST(TestFiles, test_read_text_file) {
    std::string path_text = "test/resources/file/test.txt";
    std::string content = tools::file::read_text_file(path_text);
    ASSERT_FALSE(content.empty());
    ASSERT_EQ(content, "This is a test file !");

    ASSERT_THROW(tools::file::read_text_file("no-such-file"), std::runtime_error);
}

TEST(TestFiles, test_read_binary_file) {
    std::string path_bin = "test/resources/file/test.bin";
    std::vector<uint8_t> content = tools::file::read_binary_file(path_bin);
    ASSERT_EQ(content.size(), (size_t)8);

    std::vector<uint8_t> test { 0x56, 0x20, 0x12, 0x78, 0x94, 0x65, 0x12, 0x30 };
    ASSERT_EQ(content, test);
}

TEST(TestFiles, test_write_binary_file) {
    std::vector<uint8_t> test { 0x20, 0x56, 0x78, 0x12, 0x65, 0x94, 0x30, 0x12 };

    std::string path_tmp = tools::test::get_output_path() + "/tmp.bin";
    std::filesystem::remove(path_tmp);
    ASSERT_FALSE(std::filesystem::exists(path_tmp));
    ASSERT_TRUE(tools::file::write_binary_file(test, path_tmp));
    ASSERT_EQ(tools::file::read_binary_file(path_tmp), test);
}

} // namespace test
