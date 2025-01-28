#include "gtest/gtest.h"

#include "tools/misc/UUID.hpp"

#include <algorithm>

namespace test {

using namespace tools;

TEST(TestUUID, test_ctor) {
    UUID uuid;
    ASSERT_TRUE(std::ranges::any_of(uuid.bin(), [](uint8_t byte) {
        return byte != 0;
    }));
}

TEST(TestUUID, test_str) {
    UUID uuid;
    ASSERT_TRUE(std::ranges::any_of(uuid.str(), [](char c) {
        return c != '\0';
    }));
}

TEST(TestUUID, test_comp) {
    UUID uuid;
    UUID uuid2 = uuid;
    UUID uuid3;
    ASSERT_EQ(uuid, uuid2);
    ASSERT_NE(uuid, uuid3);
}

TEST(TestUUID, test_parse) {
    UUID uuid;
    UUID uuid2(uuid.str());
    ASSERT_EQ(uuid, uuid2);
}

TEST(TestUUID, test_parse_error) {
    ASSERT_THROW(UUID("not a uuid"), std::invalid_argument);
}

} // namespace test