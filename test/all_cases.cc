#include <cstdint>
#include "md5.h"
#include "all_cases.h"
#include "gtest/gtest.h"

const uint32_t BASIC_RANGES_SIZE = 7311921;
const char BASIC_RANGES_MD5[] = "6f385dc171e201089ff96bb010b47212";

TEST(AllCases, basic_ranges_size) {
    auto &basic_ranges = BasicRanges::fetch();
    EXPECT_EQ(basic_ranges.size(), BASIC_RANGES_SIZE);
}

TEST(AllCases, basic_ranges_data) {
    auto *basic_ranges_data = new char[BASIC_RANGES_SIZE * 9];
    auto *current = basic_ranges_data;
    for (const auto &range : BasicRanges::fetch()) {
        sprintf(current, "%08X\n", range);
        current += 9;
    }
    auto basic_ranges_md5 = md5(basic_ranges_data, BASIC_RANGES_SIZE * 9);
    EXPECT_STREQ(basic_ranges_md5.c_str(), BASIC_RANGES_MD5);
}
