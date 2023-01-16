#include <stdexcept>
#include "common.h"
#include "common_code.h"

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

CommonCode::CommonCode(const RawCode &raw_code) { // load from raw code
    code = raw_code.to_common_code().code;
}

CommonCode::CommonCode(const ShortCode &short_code) { // load from short code
    code = short_code.to_common_code().code;
}

bool CommonCode::check(uint64_t common_code) { // whether common code is valid
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
