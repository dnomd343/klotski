#include "raw_code.h"
#include "common.h"

// TODO: for check function -> one 2x2 / >=2 space

/// NOTE: ensure that input raw code is valid !!!
uint64_t RawCode::compact(uint64_t raw_code) { // raw code --> common code
    int unfilled = 16;
    uint64_t head = 0; // 2x2 block address
    uint32_t range = 0;
    for (int addr = 0; raw_code; ++addr, raw_code >>= 3) { // traverse every address
        switch (raw_code & 0b111) { // low 3-bits
            case B_space:
                range <<= 2; // space
                break;
            case B_1x2:
                (range <<= 2) |= 0b01; // 1x2 block
                break;
            case B_2x1:
                (range <<= 2) |= 0b10; // 2x1 block
                break;
            case B_1x1:
                (range <<= 2) |= 0b11; // 1x1 block
                break;
            case B_2x2:
                (head = addr) <<= 32; // 2x2 block
            default:
                continue; // B_fill type
        }
        --unfilled; // unfilled number
    }
    return head | (range << (unfilled << 1)); // fill low bits as zero
}

/// NOTE: ensure that input common code is valid !!!
uint64_t RawCode::extract(uint64_t common_code) { // common code --> raw code
    auto code = C_2x2 << (common_code >> 32) * 3; // flag for 2x2 block
    auto range = Common::range_reverse((uint32_t)common_code); // reversed range
    for (int addr = 0; range; range >>= 2) {
        while (0b111 & code >> addr) { // check low 3-bits -> next empty address
            addr += 3; // found available address
        }
        switch (range & 0b11) { // match low 2-bits
            case 0b01: // 1x2 block
                code |= C_1x2 << addr;
                break;
            case 0b10: // 2x1 block
                code |= C_2x1 << addr;
                break;
            case 0b11: // 1x1 block
                code |= C_1x1 << addr;
                break;
            case 0b00: // space
                addr += 3; // next address
        }
    }
    return code;
}
