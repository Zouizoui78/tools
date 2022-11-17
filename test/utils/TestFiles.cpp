#include <gtest/gtest.h>
#include "tools/utils/Files.hpp"
#include "tools/utils/Log.hpp"

namespace test {

using namespace tools::utils::files;

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
    std::string path_text = "test_resources/Files/test.txt";
    std::string path_bin = "test_resources/Files/test.bin";
    std::string path_tmp;
};

TEST_F(TestFiles, test_read_text_file) {
    std::string content = read_text_file(path_text);
    ASSERT_FALSE(content.empty());
    ASSERT_EQ(content, "This is a test file !");
}

TEST_F(TestFiles, test_read_binary_file) {
    std::vector<uint8_t> content = read_binary_file(path_bin);
    ASSERT_EQ(content.size(), (size_t)8);

    std::vector<uint8_t> test { 0x56, 0x20, 0x12, 0x78, 0x94, 0x65, 0x12, 0x30 };
    ASSERT_EQ(content, test);
}

TEST_F(TestFiles, test_write_binary_file) {
    std::vector<uint8_t> test { 0x20, 0x56, 0x78, 0x12, 0x65, 0x94, 0x30, 0x12 };

    write_binary_file(test, path_tmp);
    ASSERT_EQ(read_binary_file(path_tmp), test);
}

} // namespace test