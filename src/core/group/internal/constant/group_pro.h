#pragma once

#include <array>

namespace klotski::cases {

// GROUP_PRO_SIZE: 1~964656 (20-bits)
// GROUP_PRO_TYPE: 0~4      (3-bits)
// GROUP_PRO_SEED: ...      (36-bits)

// seed(36) + size(20) + type(3)

constexpr auto PATTERN_DATA = std::to_array<uint64_t>({
#include "pattern.inc"
});

// seed(36) + type_id(8) + pattern_id(10) + toward(2)

constexpr auto GROUP_DATA = std::to_array<uint64_t>({
#include "group.inc"
});

} // namespace klotski::cases
