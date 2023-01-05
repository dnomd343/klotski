#include "common.h"

uint32_t Common::range_reverse(uint32_t bin) { // reverse binary every 2-bits
    bin = ((bin << 16) & 0xFFFF0000) | ((bin >> 16) & 0x0000FFFF);
    bin = ((bin << 8) & 0xFF00FF00) | ((bin >> 8) & 0x00FF00FF);
    bin = ((bin << 4) & 0xF0F0F0F0) | ((bin >> 4) & 0x0F0F0F0F);
    return ((bin << 2) & 0xCCCCCCCC) | ((bin >> 2) & 0x33333333);
}

bool Common::check_case(uint32_t head, uint32_t range) { // check whether the case is valid
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
        // TODO: should we ensure that low-bits as 0?
    }
    return true; // valid case
}
