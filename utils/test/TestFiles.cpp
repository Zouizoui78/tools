#include <gtest/gtest.h>
#include "tools/utils/Files.hpp"

namespace test {

using namespace tools::utils::files;

class TestFiles:   public ::testing::Test
{
    protected:
        TestFiles() {}

        virtual ~TestFiles() {}

        virtual void SetUp() {}

        virtual void TearDown() {}

    public:
};

TEST_F(TestFiles, test_read_text_file) {
    std::string content = read_text_file("test/test_resources/read_files/test.txt");
    ASSERT_FALSE(content.empty());
    ASSERT_EQ(content, "This is a test file !");
}

TEST_F(TestFiles, test_read_binary_file) {
    std::vector<uint8_t> content = read_binary_file<uint8_t>("test/test_resources/read_files/test.bin");
    ASSERT_EQ(content.size(), (size_t)8);

    std::vector<uint8_t> test { 0x56, 0x20, 0x12, 0x78, 0x94, 0x65, 0x12, 0x30 };
    ASSERT_EQ(content, test);

    std::vector<uint16_t> content2 = read_binary_file<uint16_t>("test/test_resources/read_files/test.bin");
    ASSERT_EQ(content2.size(), (size_t)4);

    std::vector<uint16_t> test2 { 0x2056, 0x7812, 0x6594, 0x3012 };
    ASSERT_EQ(content2, test2);

    std::vector<uint32_t> content3 = read_binary_file<uint32_t>("test/test_resources/read_files/test.bin");
    ASSERT_EQ(content3.size(), (size_t)2);

    std::vector<uint32_t> test3 { 0x78122056, 0x30126594 };
    ASSERT_EQ(content3, test3);

    std::vector<uint64_t> content4 = read_binary_file<uint64_t>("test/test_resources/read_files/test.bin");
    ASSERT_EQ(content4.size(), (size_t)1);

    std::vector<uint64_t> test4 { 0x3012659478122056 };
    ASSERT_EQ(content4, test4);
}

TEST_F(TestFiles, test_write_binary_file) {
    std::vector<uint16_t> test { 0x2056, 0x7812, 0x6594, 0x3012 };

    std::string path = "test/test_resources/read_files/tmp.bin";

    write_binary_file(test, path);
    ASSERT_EQ(read_binary_file<uint16_t>(path), test);

    std::filesystem::remove(path);
}

} // namespace test