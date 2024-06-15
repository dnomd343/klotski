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

// ------------------------------------------------------------------------------------- //

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

// ------------------------------------------------------------------------------------- //

/// The number of blocks in one klotski layout.
struct block_num_t {
    int n_1x1; // 4-bit
    int n_1x2; // 3-bit
    int n_2x1; // 3-bit
};

template <>
struct std::hash<block_num_t> {
    size_t operator()(const block_num_t val) const noexcept {
        return (val.n_1x1 << 6) ^ (val.n_1x2 << 3) ^ val.n_2x1;
    }
};

constexpr bool operator==(const block_num_t &lhs, const block_num_t &rhs) {
    return lhs.n_1x1 == rhs.n_1x1 && lhs.n_1x2 == rhs.n_1x2 && lhs.n_2x1 == rhs.n_2x1;
}

// ------------------------------------------------------------------------------------- //

/// Calculate the block number from Range.
block_num_t cal_block_num(uint32_t range);

/// Calculate type id value from the block number.
uint32_t to_type_id(block_num_t block_num);

/// Calculate the block number value from type id.
block_num_t to_block_num(uint32_t type_id);

/// Get all block number combinations without dependencies.
const std::vector<block_num_t>& block_nums();

// ------------------------------------------------------------------------------------- //

// TODO: get cases with type id (filter from AllCases)

const std::vector<CommonCode>& group_union_cases(uint32_t type_id);

// TODO: get cases with (type id + group id) (cal data from Group rules)

std::vector<CommonCode> group_cases(uint32_t type_id, uint32_t group_id);

// TODO: always return ref of `std::vector` here
