#pragma once

#include <vector>
#include <cstdint>

#include "utils/utility.h"

namespace klotski::cases {

class Ranges : public std::vector<uint32_t> {
public:
    /// Spawn klotski-ranges that match the specified block numbers.
    void spawn(int n, int n_2x1, int n_1x1);

    /// Derive the legal klotski-ranges with specified head.
    void derive(int head, Ranges &output) const;

    /// Check whether the combination of head and range is valid.
    static KLSK_INLINE int check(int head, uint32_t range);
};

typedef std::array<Ranges, 16> RangesUnion;

// TODO: add RangesUnion here
// TODO: -> spawn from Ranges / export std::vector<CommonCode>

} // namespace klotski::cases
