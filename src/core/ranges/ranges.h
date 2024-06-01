#pragma once

#include <vector>
#include <cstdint>

namespace klotski::cases {

struct bidi_t {
    uint32_t r1;
    uint32_t r2;
};

typedef std::vector<bidi_t> BidiRanges;

class Ranges : public std::vector<uint32_t> {
public:
    /// Spawn klotski-ranges that match the specified block numbers.
    void spawn(int n, int n_2x1, int n_1x1);

    /// Derive the legal klotski-ranges with specified head.
    void derive(int head, Ranges &output) const;
};

void derive_demo(const std::vector<uint32_t> &range, const std::vector<uint32_t> &reversed, std::vector<uint32_t> &output, int head);

void derive_demo_pro(const BidiRanges &bidi_range, std::vector<uint32_t> &output, int head);

typedef std::array<Ranges, 16> RangesUnion;

// TODO: add RangesUnion here
// TODO: -> spawn from Ranges / export std::vector<CommonCode>

} // namespace klotski::cases
