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
