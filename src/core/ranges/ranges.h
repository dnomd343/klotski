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
    /// Append the ranges from another instance.
    Ranges& operator+=(const Ranges &ranges);

    /// Spawn klotski-ranges that match the specified block numbers.
    void spawn(int n, int n_2x1, int n_1x1);

    /// Flip the klotski-ranges every two bits in low-high symmetry.
    void reverse();

    /// Derive the legal klotski-ranges from reversed ranges with specified head.
    void derive(int head, Ranges &output) const;

    /// Check whether the combination of head and reversed range is valid.
    static KLSK_INLINE int check(int head, uint32_t range);
};

class RangesUnion final : public std::array<Ranges, 16> {
public:
    /// Append the ranges from another instance.
    RangesUnion& operator+=(const RangesUnion &ranges_union);

    /// Export the RangesUnion as CommonCode list.
    [[nodiscard]] std::vector<codec::CommonCode> codes() const;
};

} // namespace klotski::cases
