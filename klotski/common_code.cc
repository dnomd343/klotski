#include "common.h"
#include "common_code.h"

bool CommonCode::check(uint64_t common_code) {
    uint32_t head = common_code >> 32;
    auto range = (uint32_t)common_code;
    if (head >= 16 || (head & 0b11) == 0b11) { // check 2x2 block address
        return false; // invalid common code
    }
    return Common::check_case_safe(head, Common::range_reverse(range)); // check by head and range
}
