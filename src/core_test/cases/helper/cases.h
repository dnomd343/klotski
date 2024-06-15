#pragma once

#include <array>
#include <algorithm>

#include "group/group.h"
#include "all_cases/all_cases.h"

using klotski::cases::Ranges;
using klotski::cases::AllCases;
using klotski::codec::CommonCode;
using klotski::cases::BasicRanges;
using klotski::cases::RangesUnion;

using klotski::array_sum;
using klotski::range_reverse;

using klotski::cases::BLOCK_NUM;
using klotski::cases::ALL_CASES_NUM;
using klotski::cases::ALL_CASES_NUM;
using klotski::cases::BASIC_RANGES_NUM;
using klotski::cases::BASIC_RANGES_NUM_;

/// All valid klotski heads.
constexpr auto Heads = std::to_array({
    0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14
});

/// Assert that Ranges are sorted and unique.
#define EXPECT_SORTED_AND_UNIQUE(R)                          \
    EXPECT_TRUE(std::ranges::is_sorted(R.begin(), R.end())); \
    EXPECT_EQ(std::ranges::adjacent_find(R.begin(), R.end()), R.end()) // no duplicates

/// Assert that two ordered arrays are in a containment relationship.
#define EXPECT_SUBSET(R1, R2) \
    EXPECT_TRUE(std::ranges::includes(R1.begin(), R1.end(), R2.begin(), R2.end()))

/// Assert that all CommonCodes generated from head and Ranges are valid.
#define EXPECT_COMMON_CODES(head, ranges) \
    for (const auto range : ranges)       \
        EXPECT_TRUE(CommonCode::check(static_cast<uint64_t>(head) << 32 | range))

struct block_num_t {
    int n_1x1;
    int n_1x2;
    int n_2x1;
};

block_num_t get_block_num(uint32_t range);
