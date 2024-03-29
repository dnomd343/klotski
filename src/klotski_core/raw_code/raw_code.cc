#include "common.h"
#include "raw_code.h"

namespace klotski {

RawCode::RawCode(uint64_t raw_code) {
    if (!RawCode::check(raw_code)) { // check input raw code
        throw klotski::RawCodeExp("raw code invalid");
    }
    code_ = raw_code;
}

std::ostream& operator<<(std::ostream &out, const RawCode &self) {
    char code[16];
    char dump_map[] = {
    /// 0x0  1x2  2x1  1x1  2x2  b101 b110 fill
        '.', '~', '|', '*', '@', '?', '?', '+'
    };
    sprintf(code, "%015lX", self.code_); // code length -> 15
    out << code << '\n';
    for (int addr = 0; addr < 60; addr += 3) {
        out << dump_map[(self.code_ >> addr) & 0b111];
        out << " " << &"\n"[(addr & 0b11) != 0b01];
    }
    return out;
}

bool RawCode::check(uint64_t raw_code) noexcept { // check whether raw code is valid
    ///     MASK_1x1      |     MASK_1x2      |     MASK_2x1      |     MASK_2x2
    ///  100 000 000 000  |  000 100 000 000  |  000 000 000 000  |  000 100 000 000
    ///  000 000 000 000  |  000 000 000 000  |  100 000 000 000  |  100 100 000 000
    ///  ...              |  ...              |  ...              |  ...
    ///
    constexpr uint64_t MASK_1x1 = ~B_1x1 & 0b111;
    constexpr uint64_t MASK_1x2 = MASK_1x1 << 3;
    constexpr uint64_t MASK_2x1 = MASK_1x1 << 12;
    constexpr uint64_t MASK_2x2 = MASK_1x1 << 3 | MASK_1x1 << 12 | MASK_1x1 << 15;
    if (raw_code >> 60) {
        return false; // high 4-bit must be zero
    }

    int head_num = 0, space_num = 0; // statistics for space and 2x2 number
    for (int addr = 0; addr < 20; ++addr, raw_code >>= 3) {
        switch (raw_code & 0b111) {
            case B_1x1:
            case B_fill:
                continue; // do nothing
            case B_space:
                ++space_num;
                continue;
            case B_2x1:
                if (addr > 15 || (raw_code & F_2x1) != C_2x1) {
                    return false; // invalid 2x1 block
                }
                raw_code &= ~MASK_2x1; // B_fill -> B_1x1
                continue;
            case B_1x2:
                if ((addr & 0b11) == 0b11 || (raw_code & F_1x2) != C_1x2) {
                    return false; // invalid 1x2 block
                }
                raw_code &= ~MASK_1x2; // B_fill -> B_1x1
                continue;
            case B_2x2:
                ++head_num;
                if (addr > 14 || (addr & 0b11) == 0b11 || (raw_code & F_2x2) != C_2x2) {
                    return false; // invalid 2x2 block
                }
                raw_code &= ~MASK_2x2; // B_fill -> B_1x1
                continue;
            default:
                return false; // unknown flag -> 0b101 / 0b110
        }
    }
    return head_num == 1 && space_num >= 2; // one head and at least 2 space
}

} // namespace klotski
