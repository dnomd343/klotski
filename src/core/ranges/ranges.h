/// Klotski Engine by Dnomd343 @2024

/// Range is the lower 32-bit of CommonCode, which is a combination of space,
/// 1x2, 2x1 and 1x1. They each occupy 2-bit, corresponding to binary 00, 01,
/// 10 and 11, note that the remaining positions will be filled with 0.

/// Since there is only one 2x2 block, and there are at least two spaces, this
/// means that they satisfy the following formula in number:
///
///   1. n_space + n_1x1 + (n_2x1 + n_1x2) * 2 = 5 * 4 - 2 * 2
///   2. n_2x1 != 7 (not exist on 5x4 board)
///   3. n_space >= 2
///
/// That is:
///   n_1x1 + (n_2x1 + n_1x2) * 2 <= 14   (n_2x1 != 7)
///
/// In the above inequality, the variables are all positive integers, so we can
/// get 203 combinations. In each combination, four kinds of 2-bit items can be
/// arranged in different ways to get a Range list.

/// The Range list is called Ranges, which is an array of `uint32_t`. Given the
/// specified number of n_1x2, n_2x1 and n_1x1, which can get the corresponding
/// Ranges. Normally, the Ranges is non-duplicate and sequential, but it does
/// not guarantee this to provide optimal performance.

/// Obviously, Range can be combined with 12 different head values to generate
/// [0, 12) valid CommonCodes. For a Ranges, we can match each element with all
/// head values and store the Range that can generate CommonCode according to
/// the head value. Since the head value is in [0, 16), we can store the result
/// in a Ranges array of length 16. This structure is called RangesUnion, which
/// is essentially a CommonCode array, but it can save half the space compared
/// to directly storing the `uint64_t` structure.

#pragma once

#include <array>
#include <vector>
#include <cstdint>

#include "utils/utility.h"
#include "common_code/common_code_fwd.h"

namespace klotski::cases {

class Ranges final : public std::vector<uint32_t> {
public:
    // ------------------------------------------------------------------------------------- //

    /// Append the ranges from another instance.
    Ranges& operator+=(const Ranges &ranges);

    /// Flip the ranges every two bits in low-high symmetry.
    KLSK_INLINE void reverse();

    /// Spawn klotski-ranges that match the specified block numbers.
    void spawn(int n, int n_2x1, int n_1x1);

    /// Derive the legal ranges from reversed ranges with specified head.
    void derive(int head, Ranges &output) const;

    /// Check whether the combination of head and reversed range is valid.
    static KLSK_INLINE_CE int check(int head, uint32_t range);

    // ------------------------------------------------------------------------------------- //
};

class RangesUnion final : public std::array<Ranges, 16> {
public:
    // ------------------------------------------------------------------------------------- //

    /// Get the Ranges of specified head.
    Ranges& ranges(size_t head);

    /// Get the const Ranges of specified head.
    [[nodiscard]] const Ranges& ranges(size_t head) const;

    /// Export the RangesUnion as a CommonCode list.
    [[nodiscard]] std::vector<codec::CommonCode> codes() const;

    // ------------------------------------------------------------------------------------- //

    /// Get the number of ranges contained.
    [[nodiscard]] KLSK_INLINE size_t size() const;

    /// Append the ranges from another instance.
    RangesUnion& KLSK_INLINE operator+=(const RangesUnion &ranges_union);

    /// Obtain the CommonCode of the specified index.
    [[nodiscard]] KLSK_INLINE codec::CommonCode operator[](size_type n) const;

    // ------------------------------------------------------------------------------------- //

    static constexpr auto Heads = std::to_array<uint64_t>({
        0x0, 0x1, 0x2, 0x4, 0x5, 0x6, 0x8, 0x9, 0xA, 0xC, 0xD, 0xE
    });

    // ------------------------------------------------------------------------------------- //
};

static_assert(sizeof(Ranges) == sizeof(std::vector<uint32_t>));
static_assert(sizeof(Ranges) * 16 == sizeof(RangesUnion));

} // namespace klotski::cases

#include "internal/ranges.inl"
#include "internal/check.inl"
