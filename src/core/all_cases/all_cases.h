/// Klotski Engine by Dnomd343 @2024

/// Based on the requirements of valid klotski, the 2x2 block that must exist
/// and only one, witch will occupy 4 empty slots, and the remaining 16 slots
/// will be allocated to 1x2, 2x1, 1x1 and space. Then, according to the rules
/// of CommonCode, they are coded as `00`, `01`, `10`, `11` respectively, and
/// the remaining positions are filled with 0 and stored as 32-bit variables.

/// As we all know, a space or 1x1 block will occupy 1 slot, 1x2 and 2x1 block
/// will occupy 2 slots, and together they fill 16 positions, so all possible
/// combinations can be calculated, the number is 204. Since there are no cases
/// with seven 2x1 blocks, we need to remove one combination, which leaves 203
/// situations. Therefore, each combination can produce different permutations,
/// after mathematical calculations, there are a total of `7311885` possible
/// permutations. The goal of BasicRanges is to find these permutations, sort
/// them and store in an uint32_t array.

/// AllCases is used to generate all valid CommonCodes, its works based on all
/// permutations generated by BasicRanges, which will use different 2x2 block
/// positions to check respectively. On the 5x4 chessboard, it has 12 possible
/// positions, and the numbers are distributed in 0 ~ 15, witch called `head`.
/// For the convenience of calculation, here we use an array of length 16 for
/// storage, note that the position of 3/7/11/15 will be empty.

///   00 01 02 03
///   04 05 06 07    00 01 02
///   08 09 10 11    04 05 06  <- head of 2x2 block
///   12 13 14 15    08 09 10
///   16 17 18 19    12 13 14

/// After all the work is done, we will have 29334498 cases, distributed in 16
/// arrays. Each of them is an uint32_t array storing the ranges, which can be
/// used to save memory, otherwise the 64-bit length must be consumed.

/// By the way, due to the performance considerations of the checking process,
/// the resulting data of BasicRanges will be flipped every 2-bit, which will
/// not consume too much time, but it can almost double the speed of the case
/// checking subsequent.

#pragma once

#include <mutex>

#include "utils/utility.h"
#include "ranges/ranges_fwd.h"
#include "internal/constant.inl"

namespace klotski::cases {

// ----------------------------------------------------------------------------------------- //

class BasicRanges {
public:
    /// Execute the build process.
    void build();

    /// Execute the build process without blocking.
    void build_async(Executor &&executor, Notifier &&callback);

    /// Get basic-ranges and make sure the result is available.
    const Ranges& fetch();

    /// Determine whether the basic-ranges data is available.
    [[nodiscard]] bool is_available() const;

private:
    bool available_ = false;
    std::mutex building_ {};

    /// Get static singleton variable.
    static Ranges& get_ranges();

    KLSK_INSTANCE(BasicRanges)
};

// ----------------------------------------------------------------------------------------- //

class AllCases {
public:
    /// Execute the build process.
    void build();

    /// Execute the build process without blocking.
    void build_async(Executor &&executor, Notifier &&callback);

    /// Get all-cases and make sure the result is available.
    const RangesUnion& fetch();

    /// Determine whether the all-cases data is available.
    [[nodiscard]] bool is_available() const;

private:
    bool available_ = false;
    std::mutex building_ {};

    /// Get static singleton variable.
    static RangesUnion& get_cases();

    KLSK_INSTANCE(AllCases)
};

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::cases

#include "internal/basic_ranges.inl"
#include "internal/all_cases.inl"
