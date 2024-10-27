#pragma once

#include <array>
#include <algorithm>

#include "group/group.h"
#include "helper/concurrent.h"

// ----------------------------------------------------------------------------------------- //

using klotski::cases::Ranges;
using klotski::cases::AllCases;
using klotski::codec::CommonCode;
using klotski::cases::BasicRanges;
using klotski::cases::RangesUnion;

using klotski::group::BLOCK_NUM;
using klotski::cases::ALL_CASES_NUM;
using klotski::cases::ALL_CASES_NUM_;
using klotski::cases::BASIC_RANGES_NUM;
using klotski::cases::BASIC_RANGES_NUM_;

/// All valid klotski heads.
constexpr auto Heads = std::to_array({
    0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14
});

// TODO: add NonHeads -> {3, 7, 11, 15}

// ----------------------------------------------------------------------------------------- //

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

// ----------------------------------------------------------------------------------------- //

/// Get the type_id upper limit.
uint32_t group_union_num();

/// Get the group_id upper limit.
uint32_t group_num(uint32_t type_id);

/// Get cases contained in the specified type_id.
const std::vector<CommonCode>& group_union_cases(uint32_t type_id);

/// Get cases contained in the specified type_id and group_id.
const std::vector<CommonCode>& group_cases(uint32_t type_id, uint32_t group_id);

// ----------------------------------------------------------------------------------------- //
