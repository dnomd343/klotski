#include "common.h"

uint32_t Common::range_reverse(uint32_t bin) { // reverse binary every 2-bits
    bin = ((bin << 16) & 0xFFFF0000) | ((bin >> 16) & 0x0000FFFF);
    bin = ((bin << 8) & 0xFF00FF00) | ((bin >> 8) & 0x00FF00FF);
    bin = ((bin << 4) & 0xF0F0F0F0) | ((bin >> 4) & 0x0F0F0F0F);
    return ((bin << 2) & 0xCCCCCCCC) | ((bin >> 2) & 0x33333333);
}

/// NOTE: don't check unknown ranges
uint8_t Common::check_range(uint32_t head, uint32_t range) { // check generated range
    ///   M_1x1     M_1x2     M_2x1     M_2x2
    ///  1 0 0 0   1 1 0 0   1 0 0 0   1 1 0 0
    ///  0 0 0 0   0 0 0 0   1 0 0 0   1 1 0 0
    ///  ...       ...       ...       ...
    constexpr uint32_t M_1x1 = 0b1;
    constexpr uint32_t M_1x2 = 0b11;
    constexpr uint32_t M_2x1 = 0b10001;
    constexpr uint32_t M_2x2 = 0b110011;

    int block_offset = 1;
    uint32_t cache = M_2x2 << head; // fill 2x2 block
    for (int addr = 0; range; range >>= 2, ++block_offset) { // traverse every 2-bits
        while (cache >> addr & 0b1) {
            ++addr; // search next unfilled block
        }
        switch (range & 0b11) {
            case 0b00: // space
            case 0b11: // 1x1 block
                cache |= M_1x1 << addr; // fill space or 1x1 block
                break;
            case 0b10: // 2x1 block
                if (addr > 15 || cache >> (addr + 4) & 0b1) { // invalid address
                    return block_offset; // broken block number
                }
                cache |= M_2x1 << addr; // fill 2x1 block
                break;
            case 0b01: // 1x2 block
                if ((addr & 0b11) == 0b11 || cache >> (addr + 1) & 0b1) { // invalid address
                    return block_offset; // broken block number
                }
                cache |= M_1x2 << addr; // fill 1x2 block
                break;
        }
    }
    return 0; // pass check
}


// TODO: remove after benchmark
bool Common::check_case(uint32_t head, uint32_t range) { // whether the head and range is valid
    uint32_t mask = 0b110011 << head; // fill 2x2 block
    for (int addr = 0; range; range >>= 2) { // traverse every 2-bits
        while (mask >> addr & 0b1) {
            ++addr; // search next not filled block
        }
        switch (range & 0b11) {
            case 0b00: // space block
            case 0b11: // 1x1 block
                if (addr > 19) { // invalid address
                    return false;
                }
                mask |= 0b1 << addr; // fill 1x1 block
                break;
            case 0b10: // 2x1 block
                if (addr > 15 || mask >> (addr + 4) & 0b1) { // invalid address
                    return false;
                }
                mask |= 0b10001 << addr; // fill 2x1 block
                break;
            case 0b01: // 1x2 block
                if (addr > 18 || (addr & 0b11) == 0b11 || mask >> (addr + 1) & 0b1) { // invalid address
                    return false;
                }
                mask |= 0b11 << addr; // fill 1x2 block
                break;
        }
    }
    return true; // valid case
}
