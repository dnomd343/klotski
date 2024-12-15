#pragma once

#include <string>
#include <algorithm>
#include <gtest/gtest.h>

// ----------------------------------------------------------------------------------------- //

/// Capture ostream output as string.
template <typename T>
std::string ostream_capture(T obj) {
    std::ostringstream out;
    out << obj; // ostream capture
    return out.str();
}

#define EXPECT_OSTREAM(obj, expect) EXPECT_EQ(ostream_capture(obj), expect)

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

#define EXPECT_IOTA(R)           \
    EXPECT_FALSE(R.empty());     \
    EXPECT_EQ(R.front(), 0);     \
    EXPECT_SORTED_AND_UNIQUE(R); \
    EXPECT_EQ(R.back(), R.size() - 1)

#define EXPECT_REPEAT(R, val)  \
    EXPECT_FALSE(R.empty());   \
    EXPECT_EQ(R.front(), val); \
    EXPECT_EQ(std::adjacent_find(R.begin(), R.end(), std::not_equal_to<>{}), R.end())

// ----------------------------------------------------------------------------------------- //

#define EXPECT_PLAIN_DATA(T) static_assert(      \
    std::is_nothrow_destructible_v<T>         && \
    std::is_nothrow_copy_assignable_v<T>      && \
    std::is_nothrow_move_assignable_v<T>      && \
    std::is_nothrow_copy_constructible_v<T>   && \
    std::is_nothrow_move_constructible_v<T>   && \
    std::is_trivially_destructible_v<T>       && \
    std::is_trivially_copy_assignable_v<T>    && \
    std::is_trivially_move_assignable_v<T>    && \
    std::is_trivially_copy_constructible_v<T> && \
    std::is_trivially_move_constructible_v<T>)
