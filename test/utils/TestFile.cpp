#include "gtest/gtest.h"
#include "tools/utils/file.hpp"
#include "../TestTools.hpp"

namespace test {

std::endian other_endianness() {
    if (std::endian::little == std::endian::native) {
        return std::endian::big;
    }
    else {
        return std::endian::little;
    }
}

TEST(TestFiles, test_read_all_text) {
    std::string path_text = "test/resources/file/test.txt";
    std::string content = tools::file::read_all_text(path_text);

    ASSERT_FALSE(content.empty());
    ASSERT_EQ(content, "This is a test file !");

    ASSERT_THROW(tools::file::read_all_text("no-such-file"), std::runtime_error);
}

TEST(TestFiles, test_read_all_binary_uint8) {
    std::string path_bin = "test/resources/file/test.bin";
    std::vector<uint8_t> test { 0x56, 0x20, 0x12, 0x78, 0x94, 0x65, 0x12, 0x30 };

    std::vector<uint8_t> content = tools::file::read_all_binary<uint8_t>(path_bin);

    ASSERT_EQ(content.size(), 8);
    ASSERT_EQ(content, test);
}

TEST(TestFiles, test_read_all_binary_uint16) {
    std::string path_bin = "test/resources/file/test.bin";
    std::vector<uint16_t> test { 0x2056, 0x7812, 0x6594, 0x3012 };

    std::vector<uint16_t> content = tools::file::read_all_binary<uint16_t>(path_bin);

    ASSERT_EQ(content.size(), 4);
    ASSERT_EQ(content, test);
}

TEST(TestFiles, test_read_all_binary_uint16_other_endianness) {
    std::string path_bin = "test/resources/file/test.bin";
    std::vector<uint16_t> test { 0x5620, 0x1278, 0x9465, 0x1230 };

    std::vector<uint16_t> content = tools::file::read_all_binary<uint16_t>(path_bin, other_endianness());

    ASSERT_EQ(content.size(), 4);
    ASSERT_EQ(content, test);
}

TEST(TestFiles, test_read_all_binary_uint32) {
    std::string path_bin = "test/resources/file/test.bin";
    std::vector<uint32_t> test { 0x78122056, 0x30126594 };

    std::vector<uint32_t> content = tools::file::read_all_binary<uint32_t>(path_bin);

    ASSERT_EQ(content.size(), 2);
    ASSERT_EQ(content, test);
}

TEST(TestFiles, test_read_all_binary_uint32_other_endianness) {
    std::string path_bin = "test/resources/file/test.bin";
    std::vector<uint32_t> test { 0x56201278, 0x94651230 };

    std::vector<uint32_t> content = tools::file::read_all_binary<uint32_t>(path_bin, other_endianness());

    ASSERT_EQ(content.size(), 2);
    ASSERT_EQ(content, test);
}

TEST(TestFiles, test_read_all_binary_uint64) {
    std::string path_bin = "test/resources/file/test.bin";
    std::vector<uint64_t> test { 0x3012659478122056 };

    std::vector<uint64_t> content = tools::file::read_all_binary<uint64_t>(path_bin);

    ASSERT_EQ(content.size(), 1);
    ASSERT_EQ(content, test);
}

TEST(TestFiles, test_read_all_binary_uint64_other_endianness) {
    std::string path_bin = "test/resources/file/test.bin";
    std::vector<uint64_t> test { 0x5620127894651230 };

    std::vector<uint64_t> content = tools::file::read_all_binary<uint64_t>(path_bin, other_endianness());

    ASSERT_EQ(content.size(), 1);
    ASSERT_EQ(content, test);
}

TEST(TestFiles, test_read_all_binary_float) {
    std::string path_bin = "test/resources/file/test.bin";
    std::vector<float> test { 1.18551748762e+34, 5.32588417812e-10 };

    std::vector<float> content = tools::file::read_all_binary<float>(path_bin);

    ASSERT_EQ(content.size(), 2);
    for (int i = 0 ; i < content.size() ; i++) {
        ASSERT_FLOAT_EQ(content[i], test[i]);
    }
}

TEST(TestFiles, test_read_all_binary_float_other_endianness) {
    std::string path_bin = "test/resources/file/test.bin";
    std::vector<float> test { 4.40002957804e+13, -1.15651322788e-26 };

    std::vector<float> content = tools::file::read_all_binary<float>(path_bin, other_endianness());

    ASSERT_EQ(content.size(), 2);
    for (int i = 0 ; i < content.size() ; i++) {
        ASSERT_FLOAT_EQ(content[i], test[i]);
    }
}

TEST(TestFiles, test_read_all_binary_double) {
    std::string path_bin = "test/resources/file/test.bin";
    double test = 3.9719459310071480e-77;

    std::vector<double> content = tools::file::read_all_binary<double>(path_bin);

    ASSERT_EQ(content.size(), 1);
    ASSERT_DOUBLE_EQ(content[0], test);
}

TEST(TestFiles, test_read_all_binary_double_other_endianness) {
    std::string path_bin = "test/resources/file/test.bin";
    double test = 7.3722918569420690e+106;

    std::vector<double> content = tools::file::read_all_binary<double>(path_bin, other_endianness());

    ASSERT_EQ(content.size(), 1);
    ASSERT_DOUBLE_EQ(content[0], test);
}

TEST(TestFiles, test_write_binary_uint8) {
    std::string path_tmp = test::get_output_path() + "/tmp8.bin";
    std::filesystem::remove(path_tmp);
    std::vector<uint8_t> test { 0x56, 0x20, 0x12, 0x78, 0x94, 0x65, 0x12, 0x30 };

    int ret = tools::file::write_binary<uint8_t>(path_tmp, test.begin(), test.end());

    ASSERT_EQ(ret, 8);
    ASSERT_EQ(tools::file::read_all_binary<uint8_t>(path_tmp), test);
}

TEST(TestFiles, test_write_binary_uint16) {
    std::string path_tmp = test::get_output_path() + "/tmp16.bin";
    std::filesystem::remove(path_tmp);
    std::vector<uint16_t> test { 0x2056, 0x7812, 0x6594, 0x3012 };

    int ret = tools::file::write_binary<uint16_t>(path_tmp, test.begin(), test.end());

    ASSERT_EQ(ret, 8);
    ASSERT_EQ(tools::file::read_all_binary<uint16_t>(path_tmp), test);
}

TEST(TestFiles, test_write_binary_uint16_other_endianness) {
    std::string path_tmp = test::get_output_path() + "/tmp16other.bin";
    std::filesystem::remove(path_tmp);
    std::vector<uint16_t> test { 0x2056, 0x7812, 0x6594, 0x3012 };

    int ret = tools::file::write_binary<uint16_t>(path_tmp, test.begin(), test.end(), other_endianness());

    ASSERT_EQ(ret, 8);
    ASSERT_EQ(tools::file::read_all_binary<uint16_t>(path_tmp, other_endianness()), test);
}

TEST(TestFiles, test_write_binary_uint32) {
    std::string path_tmp = test::get_output_path() + "/tmp32.bin";
    std::filesystem::remove(path_tmp);
    std::vector<uint32_t> test { 0x78122056, 0x30126594 };

    int ret = tools::file::write_binary<uint32_t>(path_tmp, test.begin(), test.end());

    ASSERT_EQ(ret, 8);
    ASSERT_EQ(tools::file::read_all_binary<uint32_t>(path_tmp), test);
}

TEST(TestFiles, test_write_binary_uint32_other_endianness) {
    std::string path_tmp = test::get_output_path() + "/tmp32other.bin";
    std::filesystem::remove(path_tmp);
    std::vector<uint32_t> test { 0x20567812, 0x65943012 };

    int ret = tools::file::write_binary<uint32_t>(path_tmp, test.begin(), test.end(), other_endianness());

    ASSERT_EQ(ret, 8);
    ASSERT_EQ(tools::file::read_all_binary<uint32_t>(path_tmp, other_endianness()), test);
}

TEST(TestFiles, test_write_binary_uint64) {
    std::string path_tmp = test::get_output_path() + "/tmp64.bin";
    std::filesystem::remove(path_tmp);
    std::vector<uint64_t> test { 0x3012659478122056 };

    int ret = tools::file::write_binary<uint64_t>(path_tmp, test.begin(), test.end());

    ASSERT_EQ(ret, 8);
    ASSERT_EQ(tools::file::read_all_binary<uint64_t>(path_tmp), test);
}

TEST(TestFiles, test_write_binary_uint64_other_endianness) {
    std::string path_tmp = test::get_output_path() + "/tmp64other.bin";
    std::filesystem::remove(path_tmp);
    std::vector<uint64_t> test { 0x2056781265943012 };

    int ret = tools::file::write_binary<uint64_t>(path_tmp, test.begin(), test.end(), other_endianness());

    ASSERT_EQ(ret, 8);
    ASSERT_EQ(tools::file::read_all_binary<uint64_t>(path_tmp, other_endianness()), test);
}

TEST(TestFiles, test_write_binary_float) {
    std::string path_tmp = test::get_output_path() + "/tmpfloat.bin";
    std::filesystem::remove(path_tmp);
    std::vector<float> test { 1.18551748762e+34, 5.32588417812e-10 };

    int ret = tools::file::write_binary<float>(path_tmp, test.begin(), test.end());

    ASSERT_EQ(ret, 8);
    auto content = tools::file::read_all_binary<float>(path_tmp);
    for (int i = 0 ; i < content.size() ; i++) {
        ASSERT_FLOAT_EQ(content[i], test[i]);
    }
}

TEST(TestFiles, test_write_binary_float_other_endianness) {
    std::string path_tmp = test::get_output_path() + "/tmpfloatother.bin";
    std::filesystem::remove(path_tmp);
    std::vector<float> test { 4.40002957804e+13, -1.15651322788e-26 };

    int ret = tools::file::write_binary<float>(path_tmp, test.begin(), test.end(), other_endianness());

    ASSERT_EQ(ret, 8);
    auto content = tools::file::read_all_binary<float>(path_tmp, other_endianness());
    for (int i = 0 ; i < content.size() ; i++) {
        ASSERT_FLOAT_EQ(content[i], test[i]);
    }
}

TEST(TestFiles, test_write_binary_double) {
    std::string path_tmp = test::get_output_path() + "/tmpdouble.bin";
    std::filesystem::remove(path_tmp);
    std::vector<double> test = { 3.9719459310071480e-77 };

    int ret = tools::file::write_binary<double>(path_tmp, test.begin(), test.end());

    ASSERT_EQ(ret, 8);
    auto content = tools::file::read_all_binary<double>(path_tmp);
    ASSERT_DOUBLE_EQ(content[0], test[0]);
}

TEST(TestFiles, test_write_binary_double_other_endianness) {
    std::string path_tmp = test::get_output_path() + "/tmpdoubleother.bin";
    std::filesystem::remove(path_tmp);
    std::vector<double> test = { 7.3722918569420690e+106 };

    int ret = tools::file::write_binary<double>(path_tmp, test.begin(), test.end(), other_endianness());

    ASSERT_EQ(ret, 8);
    auto content = tools::file::read_all_binary<double>(path_tmp, other_endianness());
    ASSERT_DOUBLE_EQ(content[0], test[0]);
}

} // namespace test
