#include "common.h"
#include "klotski.h"

uint64_t compact_code(uint64_t code) {
    int block_num = 0;
    uint64_t ret = 0;
    uint32_t range = 0;
    for (int addr = 0; code; ++addr, code >>= 3) {
        switch (code & 0x7) {
            case B_space:
                range <<= 2;
                break;
            case B_1x2:
                (range <<= 2) |= 0x1;
                break;
            case B_2x1:
                (range <<= 2) |= 0x2;
                break;
            case B_1x1:
                (range <<= 2) |= 0x3;
                break;
            case B_2x2:
                ret |= uint64_t(addr) << 32;
            default:
                continue;
        }
        ++block_num;
    }
    return ret | range << (16 - block_num) * 2;
}

uint64_t extract_code(uint64_t code) {
    uint64_t ret = C_2x2 << (code >> 32) * 3;
    auto range = uint32_t(code);
    binary_reverse(range);
    for (int addr = 0; range; range >>= 2) {
        while (0x7 & ret >> addr) {
            addr += 3;
        }
        switch (range & 0x3) {
            case 0x1:
                ret |= C_1x2 << addr;
                break;
            case 0x2:
                ret |= C_2x1 << addr;
                break;
            case 0x3:
                ret |= C_1x1 << addr;
                break;
            case 0x0:
                addr += 3;
        }
    }
    return ret;
}
