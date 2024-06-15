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

} // namespace klotski::cases
