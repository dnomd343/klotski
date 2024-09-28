#pragma once

#include <array>

namespace klotski::cases {

// TODO: should we try to compress it?
constexpr auto GROUP_SIZE = std::to_array<uint32_t>({
#include "sizes.inc"
});

constexpr auto GROUP_SEED = std::to_array<uint64_t>({
#include "seeds.inc"
});

constexpr auto GROUP_MIRROR_H = std::to_array<uint16_t>({
#include "mirror_h.inc"
});

constexpr auto GROUP_MIRROR_V = std::to_array<uint16_t>({
#include "mirror_v.inc"
});

} // namespace klotski::cases
