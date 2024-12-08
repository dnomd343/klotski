#pragma once

#include "utils/utility.h"

namespace klotski::codec {

constexpr bool CommonCode::check(const uint64_t common_code) {
    // TODO: optimization of synchronizing all_cases.

    ///   M_1x1   |   M_1x2   |   M_2x1   |   M_2x2
    ///  1 0 0 0  |  1 1 0 0  |  1 0 0 0  |  1 1 0 0
    ///  0 0 0 0  |  0 0 0 0  |  1 0 0 0  |  1 1 0 0
    ///  ...      |  ...      |  ...      |  ...
    constexpr uint32_t M_1x1 = 0b1;
    constexpr uint32_t M_1x2 = 0b11;
    constexpr uint32_t M_2x1 = 0b10001;
    constexpr uint32_t M_2x2 = 0b110011;

    /// 2x2 address check (high 32-bit)
    const uint32_t head = common_code >> 32;
    if (head >= 16 || (head & 0b11) == 0b11) { // check 2x2 block address
        return false; // invalid common code
    }

    /// check range status (low 32-bit)
    int space_num = 0;
    uint32_t mask = M_2x2 << head; // fill 2x2 block
    auto range = range_reverse((uint32_t)common_code); // extract range

    for (int addr = 0;; range >>= 2) { // traverse every 2-bit
        while ((mask >> addr) & 0b1) {
            ++addr; // search next unfilled block
        }
        if (addr >= 20) {
            return !range && space_num > 1; // range is empty and space num >= 2
        }
        switch (range & 0b11) {
            case 0b00: /// space
                mask |= M_1x1 << addr; // fill space
                ++space_num;
                continue;
            case 0b11: /// 1x1 block
                mask |= M_1x1 << addr; // fill 1x1 block
                continue;
            case 0b10: /// 2x1 block
                if (addr > 15 || mask >> (addr + 4) & 0b1) {
                    return false; // invalid address
                }
                mask |= M_2x1 << addr; // fill 2x1 block
                continue;
            case 0b01: /// 1x2 block
                if ((addr & 0b11) == 0b11 || mask >> (addr + 1) & 0b1) {
                    return false; // invalid address
                }
                mask |= M_1x2 << addr; // fill 1x2 block
                continue;
        }
    }
}

} // namespace klotski::codec
