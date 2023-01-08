#include <stdexcept>
#include "common.h"
#include "common_code.h"

/// WARN: bin should not be zero
inline uint8_t last_zero_num(uint32_t bin) { // get last zero number
    bin ^= (bin - 1);
    return __builtin_popcount(bin >> 1);
}

std::string CommonCode::to_string(uint64_t common_code, bool shorten) {
    if (!CommonCode::check(common_code)) {
        throw std::invalid_argument("invalid common code");
    }
    char result[10]; // max length 9-bits
    sprintf(result, "%09lX", common_code);
    if (shorten) { // remove `0` after common code
        if (common_code == 0x000000000) {
            return "0"; // special case
        }
        result[9 - last_zero_num(common_code) / 4] = '\0'; // truncate string
    }
    return result;
}

uint64_t CommonCode::from_string(const std::string &common_code) {
    if (common_code.length() > 9 || common_code.length() == 0) {
        throw std::invalid_argument("common code format error");
    }
    uint64_t result = 0;
    for (auto const &bit : common_code) {
        result <<= 4;
        if (bit >= '0' && bit <= '9') { // 0 ~ 9
            result |= (bit - 48);
        } else if (bit >= 'A' && bit <= 'Z') { // A ~ Z
            result |= (bit - 55);
        } else if (bit >= 'a' && bit <= 'z') { // a ~ z
            result |= (bit - 87);
        } else {
            throw std::invalid_argument("common code format error");
        }
    }
    // TODO: should we ensure that common code is valid?
    return result << (9 - common_code.length()) * 4; // low-bits fill with zero
}

bool CommonCode::check(uint64_t common_code) { // check whether common code is valid
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
