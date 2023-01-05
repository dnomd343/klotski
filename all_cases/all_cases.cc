#include <algorithm>
#include "all_cases.h"

//inline uint32_t binary_count(uint32_t bin) { // get number of non-zero bits
//    bin -= (bin >> 1) & 0x55555555;
//    bin = (bin & 0x33333333) + ((bin >> 2) & 0x33333333);
//    bin = ((bin >> 4) + bin) & 0x0F0F0F0F;
////    return (bin * 0x01010101) >> 24; // AMD CPU
//    bin += bin >> 8;
//    bin += bin >> 16;
//    return bin & 0b111111;
//}

//uint32_t AllCases::binary_reverse(uint32_t bin) { // reverse binary every 2-bits
//    bin = ((bin << 16) & 0xFFFF0000) | ((bin >> 16) & 0x0000FFFF);
//    bin = ((bin << 8) & 0xFF00FF00) | ((bin >> 8) & 0x00FF00FF);
//    bin = ((bin << 4) & 0xF0F0F0F0) | ((bin >> 4) & 0x0F0F0F0F);
//    return ((bin << 2) & 0xCCCCCCCC) | ((bin >> 2) & 0x33333333);
//}

//bool AllCases::check_case(uint32_t head, uint32_t range) { // check whether the case is valid
//    uint32_t mask = 0b110011 << head; // fill 2x2 block
//    for (int addr = 0; range; range >>= 2) { // traverse every 2-bits
//        while (mask >> addr & 0b1) {
//            ++addr; // search next not filled block
//        }
//        switch (range & 0b11) {
//            case 0b00: // space block
//            case 0b11: // 1x1 block
//                if (addr > 19) { // invalid address
//                    return false;
//                }
//                mask |= 0b1 << addr; // fill 1x1 block
//                break;
//            case 0b10: // 2x1 block
//                if (addr > 15 || mask >> (addr + 4) & 0b1) { // invalid address
//                    return false;
//                }
//                mask |= 0b10001 << addr; // fill 2x1 block
//                break;
//            case 0b01: // 1x2 block
//                if (addr > 18 || (addr & 0b11) == 0b11 || mask >> (addr + 1) & 0b1) { // invalid address
//                    return false;
//                }
//                mask |= 0b11 << addr; // fill 1x2 block
//                break;
//        }
//    }
//    return true; // valid case
//}

//void AllCases::generate_ranges(int n1, int n2, int n3, int n4) { // build target ranges
//    int len, limit;
//    constexpr uint32_t M_01 = 0b01 << 30;
//    constexpr uint32_t M_10 = 0b10 << 30;
//    constexpr uint32_t M_11 = 0b11 << 30;
//    std::vector<uint32_t> cache_1, cache_2;
//
//    len = n1 + n2;
//    limit = 0b1 << len;
//    for (uint32_t bin = 0; bin < limit; ++bin) {
//        if (binary_count(bin) != n2) { // skip binary without `n2` non-zero bits
//            continue;
//        }
//        uint32_t range = 0;
//        for (int i = 0; i < len; ++i) { // generate range base on binary value
//            range >>= 2;
//            if ((bin >> i) & 0b1) { // non-zero bit
//                range |= M_01; // 01000...
//            }
//        }
//        cache_1.emplace_back(range); // insert into first layer
//    }
//
//    len += n3;
//    limit <<= n3;
//    for (uint32_t bin = 0; bin < limit; ++bin) {
//        if (binary_count(bin) != n3) { // skip binary without `n3` non-zero bits
//            continue;
//        }
//        for (uint32_t base : cache_1) { // traverse first layer
//            uint32_t range = 0;
//            for (int i = 0; i < len; ++i) { // generate range base on binary value
//                if ((bin >> i) & 0b1) { // non-zero bit
//                    (range >>= 2) |= M_10; // 10000...
//                    continue;
//                }
//                (range >>= 2) |= base & M_11;
//                base <<= 2;
//            }
//            cache_2.emplace_back(range); // insert into second layer
//        }
//    }
//
//    len += n4;
//    limit <<= n4;
//    for (uint32_t bin = 0; bin < limit; ++bin) {
//        if (binary_count(bin) != n4) { // skip binary without `n4` non-zero bits
//            continue;
//        }
//        for (uint32_t base : cache_2) { // traverse second layer
//            uint32_t range = 0;
//            for (int i = 0; i < len; ++i) { // generate range base on binary value
//                if ((bin >> i) & 0b1) { // non-zero bit
//                    (range >>= 2) |= M_11; // 11000...
//                    continue;
//                }
//                (range >>= 2) |= base & M_11;
//                base <<= 2;
//            }
//            basic_ranges.emplace_back(range); // insert into release ranges
//        }
//    }
//}

//void AllCases::load_basic_ranges() { // load basic ranges
//    for (int n = 0; n <= 7; ++n) { // number of 1x2 and 2x1 block -> 0 ~ 7
//        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) { // number of 2x1 block -> 0 ~ n
//            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) { // number of 1x1 block -> 0 ~ (14 - 2n)
//                int n_1x2 = n - n_2x1;
//                int n_space = 16 - n * 2 - n_1x1;
//                /// 0x0 -> 00 | 1x2 -> 01 | 2x1 -> 10 | 1x1 -> 11
//                generate_ranges(n_space, n_1x2, n_2x1, n_1x1); // build target ranges
//            }
//        }
//    }
//    std::sort(basic_ranges.begin(), basic_ranges.end()); // sort basic ranges
//    for (uint32_t &range : basic_ranges) {
//        range = binary_reverse(range); // range reverse
//    }
//}

//void AllCases::find_all_cases() { // find all valid cases
//    for (uint32_t head = 0; head < 16; ++head) { // address for 2x2 block
//        if ((head & 0b11) == 0b11) {
//            continue;
//        }
//        all_cases[head].clear();
//        for (uint32_t range : basic_ranges) { // combine 2x2 address and range
//            if (check_case(head, range)) {
//                all_cases[head].emplace_back(binary_reverse(range)); // found valid case
//            }
//        }
//    }
//}
