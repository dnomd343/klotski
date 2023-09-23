#include <iostream>

#include "reverse.h"
#include "all_cases.h"

//typedef uint32_t Range;

using klotski::cases::BasicRanges;
using klotski::cases::range_reverse;

uint32_t check_range(uint32_t head, uint32_t range) noexcept {
    ///   M_1x1   |   M_1x2   |   M_2x1   |   M_2x2
    ///  1 0 0 0  |  1 1 0 0  |  1 0 0 0  |  1 1 0 0
    ///  0 0 0 0  |  0 0 0 0  |  1 0 0 0  |  1 1 0 0
    ///  ...      |  ...      |  ...      |  ...
    constexpr uint32_t M_1x1 = 0b1;
    constexpr uint32_t M_1x2 = 0b11;
    constexpr uint32_t M_2x1 = 0b10001;
    constexpr uint32_t M_2x2 = 0b110011;

    uint32_t offset = 1;
    uint32_t tmp = M_2x2 << head; // fill 2x2 block
    for (int addr = 0; range; range >>= 2, ++offset) { // traverse every 2-bits
        while ((tmp >> addr) & 0b1) {
            ++addr; // search next unfilled block
        }
        switch (range & 0b11) {
            case 0b00: /// space
            case 0b11: /// 1x1 block
                tmp |= M_1x1 << addr; // fill space or 1x1 block
                break;
            case 0b10: /// 2x1 block
                if (addr > 15 || tmp >> (addr + 4) & 0b1) { // invalid address
                    return offset; // broken block number
                }
                tmp |= M_2x1 << addr; // fill 2x1 block
                break;
            case 0b01: /// 1x2 block
                if ((addr & 0b11) == 0b11 || tmp >> (addr + 1) & 0b1) { // invalid address
                    return offset; // broken block number
                }
                tmp |= M_1x2 << addr; // fill 1x2 block
                break;
        }
    }
    return 0; // pass check
}

int check_range_v2(int head, uint32_t range) noexcept {
    constexpr uint32_t MASK_1x1 = 0b00000001;
    constexpr uint32_t MASK_1x2 = 0b00000011;
    constexpr uint32_t MASK_2x1 = 0b00010001;
    constexpr uint32_t MASK_2x2 = 0b00110011;

    uint32_t flags = MASK_2x2 << head;

    for (int addr = 0; range; range >>= 2) {

        int nn = __builtin_ctz(~flags);
        addr += nn;
        flags >>= nn;

        switch (range & 0b11) {
            case 0b00:
            case 0b11:
                flags |= MASK_1x1;
                continue;
            case 0b10:
                if ((flags >> 4) & 0b1 || addr > 15) { // invalid address
                    return -1; // broken block number
                }
                flags |= MASK_2x1;
                continue;
            case 0b01:
                if ((flags >> 1) & 0b1 || (addr & 0b11) == 0b11) { // invalid address
                    return -1; // broken block number
                }
                flags |= MASK_1x2;
                continue;
        }
    }

    return 0;
}

void demo() {

//    std::cout << check_range(1, 0x003F78CE) << std::endl;
//    std::cout << check_range_v3(1, 0x003F78CE) << std::endl;
//    return;

//    uint32_t demo = 0x61;
//    std::cout << __builtin_ctz(demo) << std::endl;
//    return;

    // 0xA9BF0C00
//    auto ret = check_range_v2(1, range_reverse(0xA9BF0C00));
//    std::cout << ret << std::endl;
//    return;
//    std::cout << ret << std::endl;

    constexpr std::array<int, 12> heads = {
        0x0, 0x1, 0x2,
        0x4, 0x5, 0x6,
        0x8, 0x9, 0xA,
        0xC, 0xD, 0xE,
    };

    auto num = 0;
    for (auto head : heads) {
        for (auto x : BasicRanges::Instance().Fetch()) {
            if (check_range_v2(head, x) == 0) {
                ++num;
            }
        }
    }


//    auto basic_ranges = BasicRanges::Instance().Fetch();
//    for (auto head : heads) {
//        for (uint32_t index = 0; index < basic_ranges.size(); ++index) {
//            auto broken_offset = check_range(head, basic_ranges[index]);
//            if (broken_offset) { // case invalid
//                auto delta = (uint32_t)1 << (32 - broken_offset * 2); // delta to next possible range
//                ///         !! -> broken
//                /// ( xx xx xx ) xx xx xx ... (reversed range)
//                ///         +1   00 00 00 ... (delta)
//                auto at_least = (range_reverse(basic_ranges[index]) & ~(delta - 1)) + delta;
//                while (range_reverse(basic_ranges[++index]) < at_least); // located next range
//                --index;
//                continue;
//            }
//            volatile auto r = range_reverse(basic_ranges[index]); // release valid cases
//        }
//    }

//    std::cout << num << std::endl;

}
