#include <string>
#include "common.h"
#include "raw_code.h"

uint64_t RawCode::unwrap() const { // get raw uint64_t code
    return code;
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

bool RawCode::check(uint64_t raw_code) {

    // TODO: check function for RawCode

    /// we can check any raw_code in this function

    // TODO: for check function -> one 2x2 / >=2 space

    // TODO: check the high 4-bits equal to zero

    return true;
}




