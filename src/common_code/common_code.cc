#include <stdexcept>
#include "common.h"
#include "common_code.h"

uint64_t CommonCode::unwrap() const {
    return code; // raw uint64_t code
}

CommonCode CommonCode::create(uint64_t common_code) {
    return CommonCode(common_code); // create from uint64_t
}

CommonCode CommonCode::unsafe_create(uint64_t common_code) { // create without check
    auto common = CommonCode(); // init directly
    common.code = common_code;
    return common;
}

CommonCode::CommonCode(uint64_t common_code) {
    if (!CommonCode::check(common_code)) { // check input common code
        throw std::invalid_argument("invalid common code");
    }
    code = common_code;
}

std::ostream& operator<<(std::ostream &out, const CommonCode &self) {
    char str[10];
    sprintf(str, "%09lX", self.code);
    out << str;
    return out;
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
        while ((mask >> addr) & 0b1) {
            ++addr; // search next unfilled block
        }
        if (addr >= 20) {
            return !range && space_num > 1; // empty range and >= 2 space
        }
        switch (range & 0b11) {
            case 0b00: /// space
                mask |= M_1x1 << addr; // fill space
                ++space_num;
                continue;
            case 0b11: /// 1x1 block
                mask |= M_1x1 << addr; // fill 1x1 block
                continue;
            case 0b10: /// 2x1 block
                if (addr > 15 || mask >> (addr + 4) & 0b1) {
                    return false; // invalid address
                }
                mask |= M_2x1 << addr; // fill 2x1 block
                continue;
            case 0b01: /// 1x2 block
                if ((addr & 0b11) == 0b11 || mask >> (addr + 1) & 0b1) {
                    return false; // invalid address
                }
                mask |= M_1x2 << addr; // fill 1x2 block
                continue;
        }
    }
}
