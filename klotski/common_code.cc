#include "common.h"
#include "common_code.h"

bool CommonCode::check(uint64_t common_code) {
    uint32_t head = common_code >> 32;
    if (head >= 16 || (head & 0b11) == 0b11) { // check 2x2 block address
        return false; // invalid common code
    }

    uint32_t fill_num = 0, space_num = 0;
    auto range = Common::range_reverse((uint32_t)common_code); // get common code range
    for (int i = 0; i < 32; i += 2) { // traverse range
        switch ((range >> i) & 0b11) {
            case 0b00: // space block
                ++space_num;
            case 0b11: // 1x1 block
                ++fill_num;
                break;
            case 0b01: // 1x2 block
            case 0b10: // 2x1 block
                fill_num += 2;
        }
        if (fill_num >= 16) { // all block filled
            break;
        }
    }
    if (space_num < 2) {
        return false; // at least 2 space
    }
    return Common::check_case(head, range); // check by head and range
}
