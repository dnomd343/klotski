#pragma once

#include <cstdint>

#include "utils/utility.h"

namespace klotski::codec {

/// This is the offset index of basic ranges, and its position (0 ~ 7311884) in
/// all basic ranges is located according to the first 12-bit (0 ~ 4095) within
/// the 32-bit `range` value.

constexpr auto RANGES_GLOBAL_OFFSET = std::to_array<uint32_t>({
    #include "constant/offset.inc"
});

/// This is the index for the range prefix. Given the case `head` and the first
/// 12-bit (0 ~ 4095) of the `range`, you can get the positions of all cases in
/// the current `head`. That is, according to the position of 2x2 block and the
/// `range`, the short code range is obtained.

constexpr auto RANGES_SUBSET_OFFSET = std::to_array({
    std::to_array<uint32_t>({
        #include "constant/offset_0x0.inc"
    }),
    std::to_array<uint32_t>({
        #include "constant/offset_0x1.inc"
    }),
    std::to_array<uint32_t>({
        #include "constant/offset_0x2.inc"
    }),
    {}, // offset_0x3

    std::to_array<uint32_t>({
        #include "constant/offset_0x4.inc"
    }),
    std::to_array<uint32_t>({
        #include "constant/offset_0x5.inc"
    }),
    std::to_array<uint32_t>({
        #include "constant/offset_0x6.inc"
    }),
    {}, // offset_0x7

    std::to_array<uint32_t>({
        #include "constant/offset_0x8.inc"
    }),
    std::to_array<uint32_t>({
        #include "constant/offset_0x9.inc"
    }),
    std::to_array<uint32_t>({
        #include "constant/offset_0xA.inc"
    }),
    {}, // offset_0xB

    std::to_array<uint32_t>({
        #include "constant/offset_0xC.inc"
    }),
    std::to_array<uint32_t>({
        #include "constant/offset_0xD.inc"
    }),
    std::to_array<uint32_t>({
        #include "constant/offset_0xE.inc"
    }),
    {}, // offset_0xF
});

} // namespace klotski::codec
