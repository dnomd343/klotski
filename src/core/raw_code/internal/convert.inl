#pragma once

#include "utils/common.h"
#include "utils/utility.h"

namespace klotski::codec {

constexpr uint64_t RawCode::compact(uint64_t raw_code) {
    int unfilled = 16;
    uint64_t head = 0; // 2x2 block address
    uint32_t range = 0;
    for (int addr = 0; raw_code; ++addr, raw_code >>= 3) { // traverse every address
        switch (raw_code & 0b111) { // low 3-bits
            case BLOCK_space:
                range <<= 2; // space
                break;
            case BLOCK_1x2:
                (range <<= 2) |= 0b01; // 1x2 block
                break;
            case BLOCK_2x1:
                (range <<= 2) |= 0b10; // 2x1 block
                break;
            case BLOCK_1x1:
                (range <<= 2) |= 0b11; // 1x1 block
                break;
            case BLOCK_2x2:
                (head = addr) <<= 32; // 2x2 block
            default:
                continue; // B_fill type
        }
        --unfilled; // unfilled number
    }
    return head | (range << (unfilled << 1)); // fill low bits as zero
}

constexpr uint64_t RawCode::extract(uint64_t common_code) {
    auto code = K_MASK_2x2 << (common_code >> 32) * 3; // flag for 2x2 block
    auto range = range_reverse((uint32_t)common_code); // reversed range

    for (int addr = 0; range; range >>= 2) {
        /// NOTE: (code >> 65) --> (code >> 1) may cause infinite loop
        while ((code >> addr) & 0b111 && addr < 60) { // check low 3-bits -> next empty address
            addr += 3; // found available address
        }
        switch (range & 0b11) { // match low 2-bits
            case 0b01: // 1x2 block
                code |= K_MASK_1x2 << addr;
                break;
            case 0b10: // 2x1 block
                code |= K_MASK_2x1 << addr;
                break;
            case 0b11: // 1x1 block
                code |= K_MASK_1x1 << addr;
                break;
            case 0b00: // space
                addr += 3; // next address
        }
    }
    return code;
}

} // namespace klotski::codec
