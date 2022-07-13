#include <gtest/gtest.h>
#include "tools/utils/Files.hpp"
#include "tools/utils/Log.hpp"

namespace test {

using namespace tools::utils::files;
static auto logger = tools::utils::new_logger("TestFiles");

class TestFiles:   public ::testing::Test
{
    protected:
    TestFiles() {
        outputs_path = std::string(std::getenv("TEST_OUTPUTS"));
        path_tmp = outputs_path + "/tmp.bin";
    }

    virtual ~TestFiles() {}

    virtual void SetUp() override {}

    virtual void TearDown() override {}

    public:
    std::string outputs_path;
    std::string path_text = "test/test_resources/files/test.txt";
    std::string path_bin = "test/test_resources/files/test.bin";
    std::string path_tmp;
};

TEST_F(TestFiles, test_read_text_file) {
    std::string content = read_text_file(path_text);
    ASSERT_FALSE(content.empty());
    ASSERT_EQ(content, "This is a test file !");
}

TEST_F(TestFiles, test_read_binary_file) {
    std::vector<uint8_t> content = read_binary_file<uint8_t>(path_bin);
    ASSERT_EQ(content.size(), (size_t)8);

    std::vector<uint8_t> test { 0x56, 0x20, 0x12, 0x78, 0x94, 0x65, 0x12, 0x30 };
    ASSERT_EQ(content, test);

    std::vector<uint16_t> content2 = read_binary_file<uint16_t>(path_bin);
    ASSERT_EQ(content2.size(), (size_t)4);

    std::vector<uint16_t> test2 { 0x2056, 0x7812, 0x6594, 0x3012 };
    ASSERT_EQ(content2, test2);

    std::vector<uint32_t> content3 = read_binary_file<uint32_t>(path_bin);
    ASSERT_EQ(content3.size(), (size_t)2);

    std::vector<uint32_t> test3 { 0x78122056, 0x30126594 };
    ASSERT_EQ(content3, test3);

    std::vector<uint64_t> content4 = read_binary_file<uint64_t>(path_bin);
    ASSERT_EQ(content4.size(), (size_t)1);

    std::vector<uint64_t> test4 { 0x3012659478122056 };
    ASSERT_EQ(content4, test4);
}

TEST_F(TestFiles, test_write_binary_file) {
    std::vector<uint16_t> test { 0x2056, 0x7812, 0x6594, 0x3012 };

    write_binary_file(test, path_tmp);
    ASSERT_EQ(read_binary_file<uint16_t>(path_tmp), test);
}

} // namespace test