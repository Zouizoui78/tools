#include "tools/utils/time.hpp"
#include "gtest/gtest.h"

namespace test {

using namespace tools::time;
using namespace std::chrono;
using namespace std::chrono_literals;

TEST(TestTime, test_str_to_time) {
    // Testing some formats.
    auto test = system_clock::time_point(1649074374s);
    ASSERT_EQ(string_to_time("2022/04/04 12:12:54", "%Y/%m/%d %H:%M:%S"), test);

    test = system_clock::time_point(1646096744s);
    ASSERT_EQ(string_to_time("010322 010544", "%d%m%y %H%M%S"), test);

    test = system_clock::time_point(1643767544s);
    ASSERT_EQ(string_to_time("020222 020544", "%d%m%y %H%M%S"), test);

    // Testing every days in a month.
    std::string date = "0322 010544";
    std::string date_format = "%d%m%y %H%M%S";
    auto date_start = std::chrono::system_clock::time_point(1646096744s);

    for (int i = 1; i <= 31; ++i) {
        std::string to_test = std::to_string(i) + date;
        if (i < 10) {
            to_test = "0" + to_test;
        }
        ASSERT_EQ(string_to_time(to_test, date_format),
                  date_start + 86400s * (i - 1));
    }
}

} // namespace test
