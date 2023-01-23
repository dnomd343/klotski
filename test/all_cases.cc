#include <cstdint>
#include "all_cases.h"
#include "gtest/gtest.h"

const uint32_t BASIC_RANGES_SIZE = 7311921;

TEST(AllCases, basic_ranges_size) {
    auto &basic_ranges = BasicRanges::fetch();
    EXPECT_EQ(basic_ranges.size(), BASIC_RANGES_SIZE);
}
