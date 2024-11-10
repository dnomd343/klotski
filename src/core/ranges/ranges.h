/// Klotski Engine by Dnomd343 @2024

// TODO: A stable Ranges or RangesUnion must be ordered and non-repeating.

#pragma once

#include <array>
#include <vector>
#include <cstdint>

#include "utils/utility.h"

namespace klotski::codec {
class CommonCode;
} // namespace klotski::codec

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
    static KLSK_INLINE int check(int head, uint32_t range);

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

private:
    static constexpr auto Heads = std::to_array<uint64_t>({
        0x0, 0x1, 0x2, 0x4, 0x5, 0x6, 0x8, 0x9, 0xA, 0xC, 0xD, 0xE,
    });
};

} // namespace klotski::cases

#include "internal/ranges.inl"
#include "internal/ranges_union.inl"
