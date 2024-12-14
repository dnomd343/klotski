#pragma once

#include "utils/common.h"

namespace klotski::codec {

constexpr bool RawCode::check(uint64_t raw_code) {
    ///     MASK_1x1      |     MASK_1x2      |     MASK_2x1      |     MASK_2x2
    ///  100 000 000 000  |  000 100 000 000  |  000 000 000 000  |  000 100 000 000
    ///  000 000 000 000  |  000 000 000 000  |  100 000 000 000  |  100 100 000 000
    ///  ...              |  ...              |  ...              |  ...
    ///
    constexpr uint64_t MASK_1x1 = ~BLOCK_1x1 & 0b111;
    constexpr uint64_t MASK_1x2 = MASK_1x1 << 3;
    constexpr uint64_t MASK_2x1 = MASK_1x1 << 12;
    constexpr uint64_t MASK_2x2 = MASK_1x1 << 3 | MASK_1x1 << 12 | MASK_1x1 << 15;
    if (raw_code >> 60) {
        return false; // high 4-bit must be zero
    }

    int head_num = 0, space_num = 0; // statistics for space and 2x2 number
    for (int addr = 0; addr < 20; ++addr, raw_code >>= 3) {
        switch (raw_code & 0b111) {
            case BLOCK_1x1:
            case BLOCK_fill:
                continue; // do nothing
            case BLOCK_space:
                ++space_num;
                continue;
            case BLOCK_2x1:
                if (addr > 15 || (raw_code & K_MASK_2x1_) != K_MASK_2x1) {
                    return false; // invalid 2x1 block
                }
                raw_code &= ~MASK_2x1; // B_fill -> B_1x1
                continue;
            case BLOCK_1x2:
                if ((addr & 0b11) == 0b11 || (raw_code & K_MASK_1x2_) != K_MASK_1x2) {
                    return false; // invalid 1x2 block
                }
                raw_code &= ~MASK_1x2; // B_fill -> B_1x1
                continue;
            case BLOCK_2x2:
                ++head_num;
                if (addr > 14 || (addr & 0b11) == 0b11 || (raw_code & K_MASK_2x2_) != K_MASK_2x2) {
                    return false; // invalid 2x2 block
                }
                raw_code &= ~MASK_2x2; // B_fill -> B_1x1
                continue;
            default:
                return false; // unknown flag -> 0b101 / 0b110
        }
    }
    return head_num == 1 && space_num >= 2; // one head and at least 2 space
}

} // namespace klotski::codec
