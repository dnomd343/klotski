#pragma once

#include <vector>
#include <cstdint>

namespace klotski::cases {

class Ranges : public std::vector<uint32_t> {
public:
    /// Spawn klotski-ranges that match the specified block numbers.
    void spawn(int n, int n_2x1, int n_1x1);

    /// Derive the legal klotski-ranges with specified head.
    void derive(int head, Ranges &output) const;
};

// TODO: add RangesUnion here
// TODO: -> spawn from Ranges / export std::vector<CommonCode>

} // namespace klotski::cases
