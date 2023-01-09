#include <string>
#include "common.h"
#include "raw_code.h"

uint64_t RawCode::unwrap() const { // get raw uint64_t code
    return code;
}

CommonCode RawCode::to_common_code() const {
    uint32_t range = 0;
    uint64_t raw_code = code;
    uint32_t unfilled_num = 16;
    uint64_t head = ((uint64_t)16 << 32); // using invalid address

    for (int addr = 0; raw_code; ++addr, raw_code >>= 3) {
        switch (raw_code & 0b111) { // get low 3-bits
            case B_space:
                range <<= 2; // add space block
                break;
            case B_1x2:
                (range <<= 2) |= 0b01; // add 1x2 block
                break;
            case B_2x1:
                (range <<= 2) |= 0b10; // add 2x1 block
                break;
            case B_1x1:
                (range <<= 2) |= 0b11; // add 1x1 block
                break;
            case B_2x2:
                head = (uint64_t)addr << 32; // 2x2 block address
            default:
                // TODO: should we throw an error?
                continue; // unknown block type
        }
        --unfilled_num; // less unfilled block
    }
    // TODO: should we check the high 4-bits equal to zero?

//    printf("head -> %09lX\n", head);
//    printf("range -> %08X\n", range << 10);
//    printf("unfilled -> %d\n", unfilled_num);

    unfilled_num <<= 1; // aka unfilled_num *= 2
    return CommonCode(head | (range << unfilled_num));
}

RawCode::RawCode(const CommonCode &common_code) {
    auto common_code_raw = common_code.unwrap(); // common code with uint64_t format
    code = C_2x2 << (common_code_raw >> 32) * 3; // flag for 2x2 block
    auto range = Common::range_reverse((uint32_t)common_code_raw); // load reversed range

    for (int addr = 0; range; range >>= 2) {
        while (0b111 & code >> addr) { // check low 3-bits -> until empty address
            addr += 3; // found available address
        }
        switch (range & 0b11) { // match low 2-bits
            case 0x1:
                code |= C_1x2 << addr; // add 1x2 block
                break;
            case 0x2:
                code |= C_2x1 << addr; // add 2x1 block
                break;
            case 0x3:
                code |= C_1x1 << addr; // add 1x1 block
                break;
            case 0x0:
                addr += 3; // next address
        }
    }
}

std::string RawCode::dump_case() const {

    // TODO: result reserve space -> result length
    std::string result;

    auto raw_code = code;

    for (int addr = 0; raw_code; ++addr, raw_code >>= 3) {
        switch (raw_code & 0b111) {
            case B_space:
                result.push_back('.');
                break;
            case B_fill:
                result.push_back('+');
                break;
            case B_1x1:
                result.push_back('*');
                break;
            case B_1x2:
                result.push_back('~');
                break;
            case B_2x1:
                result.push_back('|');
                break;
            case B_2x2:
                result.push_back('@');
                break;
            default:
                result.push_back('?');
        }
        if (addr % 4 == 3) {
            result.push_back('\n');
        } else {
            result.push_back(' ');
        }
    }

    return result;
}
