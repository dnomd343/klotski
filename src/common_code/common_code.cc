#include <stdexcept>
#include "common.h"
#include "common_code.h"

inline uint32_t binary_count(uint32_t bin) { // get number of non-zero bits
    bin -= (bin >> 1) & 0x55555555;
    bin = (bin & 0x33333333) + ((bin >> 2) & 0x33333333);
    bin = ((bin >> 4) + bin) & 0x0F0F0F0F;
    bin += bin >> 8;
    bin += bin >> 16;
    return bin & 0b111111;
}

/// NOTE: bin should not be zero
inline uint32_t last_zero_num(uint32_t bin) { // get last zero number
    bin ^= (bin - 1);
    return __builtin_popcount(bin >> 1);
}

uint64_t CommonCode::unwrap() const { // get raw uint64_t code
    return code;
}

RawCode CommonCode::to_raw_code() const { // convert to raw code
    return RawCode(*this);
}

ShortCode CommonCode::to_short_code() const { // convert to short code
    return ShortCode(*this);
}

CommonCode CommonCode::unsafe_create(uint64_t code) { // create CommonCode without check
    auto common_code = CommonCode(); // init directly
    common_code.code = code;
    return common_code;
}

CommonCode::CommonCode(uint64_t common_code) {
    if (!CommonCode::check(common_code)) { // check input common code
        throw std::invalid_argument("invalid common code");
    }
    code = common_code;
}

CommonCode::CommonCode(const RawCode &raw_code) { // init from raw code
    code = raw_code.to_common_code().code;
}

CommonCode::CommonCode(const ShortCode &short_code) { // init from short code
    code = short_code.to_common_code().code;
}

CommonCode::CommonCode(const std::string &common_code_str) {
    if (common_code_str.length() > 9 || common_code_str.length() == 0) { // check string length
        throw std::invalid_argument("common code format error");
    }

    uint64_t common_code = 0;
    for (auto const &bit : common_code_str) {
        common_code <<= 4;
        if (bit >= '0' && bit <= '9') { // 0 ~ 9
            common_code |= (bit - 48);
        } else if (bit >= 'A' && bit <= 'Z') { // A ~ Z
            common_code |= (bit - 55);
        } else if (bit >= 'a' && bit <= 'z') { // a ~ z
            common_code |= (bit - 87);
        } else {
            throw std::invalid_argument("common code format error"); // unknown characters
        }
    }
    common_code <<= (9 - common_code_str.length()) * 4; // low-bits fill with zero

    if (!CommonCode::check(common_code)) { // check converted common code
        throw std::invalid_argument("invalid common code");
    }
    code = common_code;
}

std::string CommonCode::to_string(bool shorten) const { // convert uint64_t code to string
    char result[10]; // max length 9-bits
    sprintf(result, "%09lX", code);
    if (shorten) { // remove `0` after common code
        if (code == 0x000000000) {
            return "0"; // special case -> only one `0`
        }
        result[9 - last_zero_num(code) / 4] = '\0'; // truncate string
    }
    return result; // char* -> std::string
}

bool CommonCode::check(uint64_t common_code) { // check whether common code is valid
    uint32_t head = common_code >> 32;
    if (head >= 16 || (head & 0b11) == 0b11) { // check 2x2 block address
        return false; // invalid common code
    }

    /// ensure that there are >= 2 space blocks
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

bool CommonCode::check_demo(uint64_t common_code) {
    ///   M_1x1     M_1x2     M_2x1     M_2x2
    ///  1 0 0 0   1 1 0 0   1 0 0 0   1 1 0 0
    ///  0 0 0 0   0 0 0 0   1 0 0 0   1 1 0 0
    ///  ...       ...       ...       ...
    constexpr uint32_t M_1x1 = 0b1;
    constexpr uint32_t M_1x2 = 0b11;
    constexpr uint32_t M_2x1 = 0b10001;
    constexpr uint32_t M_2x2 = 0b110011;

    /// 2x2 address check (high 32-bits)
    uint32_t head = common_code >> 32;
    if (head >= 16 || (head & 0b11) == 0b11) { // check 2x2 block address
        return false; // invalid common code
    }

    /// check range status (low 32-bits)
    int space_num = 0;
    uint32_t mask = M_2x2 << head; // fill 2x2 block
    auto range = Common::range_reverse((uint32_t)common_code);
    for (int addr = 0;; range >>= 2) { // traverse every 2-bits
        while (mask >> addr & 0b1) {
            ++addr; // search next unfilled block
        }
        if (addr >= 20) {
            return !range && space_num > 1; // empty range and >= 2 space
        }
        switch (range & 0b11) {
            case 0b00: // space
                ++space_num;
            case 0b11: // 1x1 block
                mask |= M_1x1 << addr; // fill space or 1x1 block
                break;
            case 0b10: // 2x1 block
                if (addr > 15 || mask >> (addr + 4) & 0b1) { // invalid address
                    return false;
                }
                mask |= M_2x1 << addr; // fill 2x1 block
                break;
            case 0b01: // 1x2 block
                if ((addr & 0b11) == 0b11 || mask >> (addr + 1) & 0b1) { // invalid address
                    return false;
                }
                mask |= M_1x2 << addr; // fill 1x2 block
                break;
        }
    }
}
