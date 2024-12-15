#pragma once

#include <array>

namespace klotski::group {

/// Properties of all patterns as follows:
///   (H)  5-bit ~ Padding for u64.
///       36-bit ~ The seed of the group with the Toward::A.
///       20-bit ~ The number of cases in the included groups.
///   (L)  3-bit ~ MirrorType enumeration of the included groups.
constexpr auto PATTERN_DATA = std::to_array<uint64_t>({
#include "pattern.inc"
});

/// Properties of all groups as follows:
///   (H)  8-bit ~ Padding for u64.
///       36-bit ~ The seed of current group.
///        8-bit ~ The type_id of current group.
///       10-bit ~ The pattern_id of current group.
///   (L)  2-bit ~ Toward enumeration of current group.
constexpr auto GROUP_DATA = std::to_array<uint64_t>({
#include "group.inc"
});

} // namespace klotski::group
