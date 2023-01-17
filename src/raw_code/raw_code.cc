#include <string>
#include "common.h"
#include "raw_code.h"

uint64_t RawCode::unwrap() const { // get raw uint64_t code
    return code;
}

#include <iostream>

/// we can check any raw_code in this function
bool RawCode::check(uint64_t raw_code) {

    if (raw_code >> 60) {
        return false; // high 4-bits must be zero
    }

    int head_num = 0;
    int space_num = 0;

    for (int addr = 0; addr < 20; ++addr, raw_code >>= 3) {

//        std::cout << "addr = " << addr << " | block = " << (raw_code & 0b111) << std::endl;

        switch (raw_code & 0b111) {
            case B_space:
                ++space_num;
                break;
            case B_1x1:
                /// do nothing
                break;
            case B_1x2:

                if ((addr & 0b11) == 0b11 || (raw_code & F_1x2) != C_1x2) {
                    return false;
                }

                break;
            case B_2x1:

                if (addr > 15 || (raw_code & F_2x1) != C_2x1) {
                    return false;
                }

                break;
            case B_2x2:
                ++head_num;

                if (addr > 14 || (addr & 0b11) == 0b11 || (raw_code & F_2x2) != C_2x2) {
                    return false;
                }

                break;

            case B_fill:
                /// do nothing
                break;

            default:
                return false; // unknown flag
        }

    }

    // TODO: one 2x2 / >=2 space

    // TODO: check head_num and space_num

    if (head_num != 1 || space_num < 2) {
        return false;
    }

    return true;
}

std::string RawCode::dump_case() const {
    std::string result;
    result.reserve(40); // 5 lines * ("x x x x\n")
    char dump_map[] = {
    /// 0x0  1x2  2x1  1x1  2x2  b101 b110 fill
        '.', '~', '|', '*', '@', '?', '?', '+'
    };

    auto raw_code = code;
    for (int addr = 0; addr < 20; ++addr, raw_code >>= 3) {
        result.push_back(dump_map[raw_code & 0b111]);
        if ((addr & 0b11) == 0b11) {
            result.push_back('\n'); // new line
        } else {
            result.push_back(' '); // add space
        }
    }
    return result;
}
