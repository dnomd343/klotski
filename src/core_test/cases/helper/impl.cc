#include <cstdint>

#include "cases.h"
#include "utils/utility.h"

block_num_t get_block_num(uint32_t range) {
    block_num_t result {};
    range = klotski::range_reverse(range);
    for (; range; range >>= 2) {
        switch (range & 0b11) {
        case 0b01: // 1x2 block
            ++result.n_1x2;
            continue;
        case 0b10: // 2x1 block
            ++result.n_2x1;
            continue;
        case 0b11: // 1x1 block
            ++result.n_1x1;
            continue;
        }
    }
    return result;
}
