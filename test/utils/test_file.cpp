#include "../test_tools.hpp"
#include "tools/utils/file.hpp"
#include "gtest/gtest.h"

#include "tools/utils/stopwatch.hpp"
#include <deque>
#include <iostream>

namespace test {

TEST(TestFile, test_read_all_text) {
    std::string path_text = "test/resources/file/test.txt";
    std::string content = tools::file::read_all_text(path_text);

    ASSERT_FALSE(content.empty());
    ASSERT_EQ(content, "This is a test file !");

    ASSERT_THROW(tools::file::read_all_text("no-such-file"),
                 std::runtime_error);
}

TEST(TestFile, test_read_all_binary_uint8) {
    std::string path_bin = "test/resources/file/test.bin";
    std::vector<uint8_t> test{0x56, 0x20, 0x12, 0x78, 0x94, 0x65, 0x12, 0x30};

    std::vector<uint8_t> content =
        tools::file::read_all_binary<uint8_t>(path_bin);

    ASSERT_EQ(content.size(), 8);
    ASSERT_EQ(content, test);
}

TEST(TestFile, test_read_all_binary_uint16) {
    std::string path_bin = "test/resources/file/test.bin";
    std::vector<uint16_t> test{0x2056, 0x7812, 0x6594, 0x3012};

    std::vector<uint16_t> content =
        tools::file::read_all_binary<uint16_t>(path_bin);

    ASSERT_EQ(content.size(), 4);
    ASSERT_EQ(content, test);
}

TEST(TestFile, test_read_all_binary_uint32) {
    std::string path_bin = "test/resources/file/test.bin";
    std::vector<uint32_t> test{0x78122056, 0x30126594};

    std::vector<uint32_t> content =
        tools::file::read_all_binary<uint32_t>(path_bin);

    ASSERT_EQ(content.size(), 2);
    ASSERT_EQ(content, test);
}

TEST(TestFile, test_read_all_binary_uint64) {
    std::string path_bin = "test/resources/file/test.bin";
    std::vector<uint64_t> test{0x3012659478122056};

    std::vector<uint64_t> content =
        tools::file::read_all_binary<uint64_t>(path_bin);

    ASSERT_EQ(content.size(), 1);
    ASSERT_EQ(content, test);
}

TEST(TestFile, test_read_all_binary_float) {
    std::string path_bin = "test/resources/file/test.bin";
    std::vector<float> test{1.18551748762e+34, 5.32588417812e-10};

    std::vector<float> content = tools::file::read_all_binary<float>(path_bin);

    ASSERT_EQ(content.size(), 2);
    for (int i = 0; i < content.size(); i++) {
        ASSERT_FLOAT_EQ(content[i], test[i]);
    }
}

TEST(TestFile, test_read_all_binary_double) {
    std::string path_bin = "test/resources/file/test.bin";
    double test = 3.9719459310071480e-77;

    std::vector<double> content =
        tools::file::read_all_binary<double>(path_bin);

    ASSERT_EQ(content.size(), 1);
    ASSERT_DOUBLE_EQ(content[0], test);
}

TEST(TestFile, test_dump_binary_uint8) {
    std::string path_tmp = test::get_output_path() + "/tmp8.bin";
    std::filesystem::remove(path_tmp);
    std::vector<uint8_t> test{0x56, 0x20, 0x12, 0x78, 0x94, 0x65, 0x12, 0x30};

    int ret = tools::file::dump_range(path_tmp, test);

    ASSERT_EQ(ret, 8);
    ASSERT_EQ(tools::file::read_all_binary<uint8_t>(path_tmp), test);
}

TEST(TestFile, test_dump_binary_uint16) {
    std::string path_tmp = test::get_output_path() + "/tmp16.bin";
    std::filesystem::remove(path_tmp);
    std::vector<uint16_t> test{0x2056, 0x7812, 0x6594, 0x3012};

    int ret = tools::file::dump_range(path_tmp, test);

    ASSERT_EQ(ret, 8);
    ASSERT_EQ(tools::file::read_all_binary<uint16_t>(path_tmp), test);
}

TEST(TestFile, test_dump_binary_uint32) {
    std::string path_tmp = test::get_output_path() + "/tmp32.bin";
    std::filesystem::remove(path_tmp);
    std::vector<uint32_t> test{0x78122056, 0x30126594};

    int ret = tools::file::dump_range(path_tmp, test);

    ASSERT_EQ(ret, 8);
    ASSERT_EQ(tools::file::read_all_binary<uint32_t>(path_tmp), test);
}

TEST(TestFile, test_dump_binary_uint64) {
    std::string path_tmp = test::get_output_path() + "/tmp64.bin";
    std::filesystem::remove(path_tmp);
    std::vector<uint64_t> test{0x3012659478122056};

    int ret = tools::file::dump_range(path_tmp, test);

    ASSERT_EQ(ret, 8);
    ASSERT_EQ(tools::file::read_all_binary<uint64_t>(path_tmp), test);
}

TEST(TestFile, test_dump_binary_float) {
    std::string path_tmp = test::get_output_path() + "/tmpfloat.bin";
    std::filesystem::remove(path_tmp);
    std::vector<float> test{1.18551748762e+34, 5.32588417812e-10};

    int ret = tools::file::dump_range(path_tmp, test);

    ASSERT_EQ(ret, 8);
    auto content = tools::file::read_all_binary<float>(path_tmp);
    for (int i = 0; i < content.size(); i++) {
        ASSERT_FLOAT_EQ(content[i], test[i]);
    }
}

TEST(TestFile, test_dump_binary_double) {
    std::string path_tmp = test::get_output_path() + "/tmpdouble.bin";
    std::filesystem::remove(path_tmp);
    std::vector<double> test{3.9719459310071480e-77};

    int ret = tools::file::dump_range(path_tmp, test);

    ASSERT_EQ(ret, 8);
    auto content = tools::file::read_all_binary<double>(path_tmp);
    ASSERT_DOUBLE_EQ(content[0], test[0]);
}

TEST(TestFile, test_benchmark_dump_contiguous) {
    std::string path_tmp = test::get_output_path() + "/tmpbenchmark.bin";
    std::filesystem::remove(path_tmp);
    std::vector<int> data;
    int size = 123456;
    data.reserve(size);
    for (int i = 0; i < size; i++) {
        data.push_back(i);
    }

    tools::utils::Stopwatch s;
    int ret = tools::file::dump_range(path_tmp, data);
    std::cout << s.get_duration<std::chrono::microseconds>() << std::endl;

    ASSERT_EQ(ret, size * sizeof(int));
}

TEST(TestFile, test_benchmark_dump_non_contiguous) {
    std::string path_tmp = test::get_output_path() + "/tmpnoncontiguous.bin";
    std::filesystem::remove(path_tmp);
    std::deque<int> data;
    int size = 123456;
    for (int i = 0; i < size; i++) {
        data.push_back(i);
    }

    tools::utils::Stopwatch s;
    int ret = tools::file::dump_range(path_tmp, data);
    std::cout << s.get_duration<std::chrono::microseconds>() << std::endl;

    ASSERT_EQ(ret, size * sizeof(int));
}

} // namespace test
