#include "common.h"

void binary_reverse(uint32_t &range) { // reverse binary every 2 bits
    range = ((range << 16) & 0xFFFF0000) | ((range >> 16) & 0x0000FFFF);
    range = ((range << 8) & 0xFF00FF00) | ((range >> 8) & 0x00FF00FF);
    range = ((range << 4) & 0xF0F0F0F0) | ((range >> 4) & 0x0F0F0F0F);
    range = ((range << 2) & 0xCCCCCCCC) | ((range >> 2) & 0x33333333);
}

void binary_to_str(uint64_t binary, char *string) {
    for (int i = 0; i < 9; ++i, ++string) { // only read low 9 * 4 bits
        *string = int8_t(binary >> (8 - i) * 4 & 0xF);
        if (*string < 10) {
            *string += 48; // 0 -> 48
        } else {
            *string += 55; // A -> 65
        }
    }
    string[9] = 0x0; // string ending
}
