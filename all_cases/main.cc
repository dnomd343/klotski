#include <iostream>
#include <vector>
#include <algorithm>

void load_basic_ranges(std::vector<uint32_t> &basic_ranges);
void build_basic_ranges(int n1, int n2, int n3, int n4, std::vector<uint32_t> &ranges);

inline uint32_t binary_count(uint32_t bin) { // get number of non-zero bits
    bin -= (bin >> 1) & 0x55555555;
    bin = (bin & 0x33333333) + ((bin >> 2) & 0x33333333);
    bin = ((bin >> 4) + bin) & 0x0F0F0F0F;
//    return (bin * 0x01010101) >> 24; // AMD CPU
    bin += bin >> 8;
    bin += bin >> 16;
    return bin & 0b111111;
}

uint32_t binary_reverse(uint32_t &bin) { // reverse binary every 2-bits
    bin = ((bin << 16) & 0xFFFF0000) | ((bin >> 16) & 0x0000FFFF);
    bin = ((bin << 8) & 0xFF00FF00) | ((bin >> 8) & 0x00FF00FF);
    bin = ((bin << 4) & 0xF0F0F0F0) | ((bin >> 4) & 0x0F0F0F0F);
    return ((bin << 2) & 0xCCCCCCCC) | ((bin >> 2) & 0x33333333);
}

bool check_case(uint32_t head, uint32_t range) { // check whether the case is valid
    uint32_t status = 0b110011 << head; // fill 2x2 block
    for (int addr = 0; range; range >>= 2) { // traverse every 2-bits
        while (status >> addr & 0b1) {
            ++addr; // search next not filled block
        }
        switch (range & 0b11) {
            case 0b00: // space block
            case 0b11: // 1x1 block
                if (addr > 19) { // invalid address
                    return false;
                }
                status |= 0b1 << addr; // fill 1x1 block
                break;
            case 0b10: // 2x1 block
                if (addr > 15 || status >> (addr + 4) & 0b1) { // invalid address
                    return false;
                }
                status |= 0b10001 << addr; // fill 2x1 block
                break;
            case 0b01: // 1x2 block
                if (addr > 18 || (addr & 0b11) == 0b11 || status >> (addr + 1) & 0b1) { // invalid address
                    return false;
                }
                status |= 0b11 << addr; // fill 1x2 block
                break;
        }
    }
    return true;
}

void find_all_cases(std::vector<uint64_t> &all_cases) {
    std::vector<uint32_t> basic_ranges, basic_rev_ranges;
    load_basic_ranges(basic_ranges);
    for (uint32_t range : basic_ranges) {
        basic_rev_ranges.emplace_back(binary_reverse(range));
    }

    all_cases.clear();
    for (uint32_t head = 0; head < 16; ++head) { // address for 2x2 block
        if ((head & 0b11) == 0b11) { // aka (head % 4 == 3)
            continue;
        }
        uint64_t prefix = int64_t(head) << 32;
        for (uint32_t range : basic_rev_ranges) { // combine 2x2 address and range
            if (check_case(head, range)) {
                // TODO: emplace with range_rev
                all_cases.emplace_back(prefix | range);
            }
        }
    }

}

void load_basic_ranges(std::vector<uint32_t> &basic_ranges) { // load basic ranges
    basic_ranges.clear();
    for (int n = 0; n <= 7; ++n) { // number of 1x2 and 2x1 block -> 0 ~ 7
        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) { // number of 2x1 block -> 0 ~ n
            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) { // number of 1x1 block -> 0 ~ (14 - 2n)
                int n_1x2 = n - n_2x1;
                int n_space = 16 - n * 2 - n_1x1;
                // 0x0 -> 00 / 1x2 -> 01 / 2x1 -> 10 / 1x1 -> 11
                build_basic_ranges(n_space, n_1x2, n_2x1, n_1x1, basic_ranges);
            }
        }
    }
    std::sort(basic_ranges.begin(), basic_ranges.end()); // sort basic ranges
}

void build_basic_ranges(int n1, int n2, int n3, int n4, std::vector<uint32_t> &ranges) {
//    printf("generate n1 = %d | n2 = %d | n3 = %d | n4 = %d\n", n1, n2, n3, n4);

    constexpr uint32_t M_01 = 0b01 << 30;
    constexpr uint32_t M_10 = 0b10 << 30;
    constexpr uint32_t M_11 = 0b11 << 30;

    std::vector<uint32_t> cache_1;
    std::vector<uint32_t> cache_2;

    int len, limit;

    len = n1 + n2;
    limit = 0b1 << len;

    for (uint32_t bin = 0; bin < limit; ++bin) {
        if (binary_count(bin) != n2) { // skip binary without `n2` non-zero bits
            continue;
        }
        uint32_t range = 0;
        uint32_t mask = bin;

        for (int i = 0; i < len; ++i) { // generate range base on binary value
            range >>= 2;
            if (mask & 0b1) { // non-zero bit
                range |= M_01;
            }
            mask >>= 1;
        }
        cache_1.emplace_back(range); // insert into cache level 1
    }

    len += n3;
    limit <<= n3;

    for (uint32_t bin = 0; bin < limit; ++bin) {
        if (binary_count(bin) != n3) { // skip binary without `n3` non-zero bits
            continue;
        }

        for (uint32_t base : cache_1) { // traverse cache level 1

            uint32_t range = 0;
            uint32_t mask = bin;

            for (int i = 0; i < len; ++i) { // generate range base on binary value
                range >>= 2;
                if (mask & 0b1) { // non-zero bit
                    range |= M_10;
                } else { // zero bit
                    range |= base & M_11;
                    base <<= 2;
                }
                mask >>= 1;
            }
            cache_2.emplace_back(range); // insert into cache level 2

        }

    }


    len += n4;
    limit <<= n4;

    for (uint32_t bin = 0; bin < limit; ++bin) {
        if (binary_count(bin) != n4) { // skip binary without `n4` non-zero bits
            continue;
        }
        for (uint32_t base : cache_2) { // traverse cache level 2
            uint32_t range = 0;
            uint32_t mask = bin;

            for (int i = 0; i < len; ++i) { // generate range base on binary value
                range >>= 2;
                if (mask & 0b1) { // non-zero bit
                    range |= M_11;
                } else { // zero bit
                    range |= base & M_11;
                    base <<= 2;
                }
                mask >>= 1;
            }
            ranges.emplace_back(range); // insert into release ranges

        }
    }

}

//bool check_case(uint32_t head, uint32_t range) { // whether case is valid
//
//    constexpr uint32_t M_00 = 0B00 << 30;
//    constexpr uint32_t M_01 = 0b01 << 30;
//    constexpr uint32_t M_10 = 0b10 << 30;
//    constexpr uint32_t M_11 = 0b11 << 30;
//
//    uint32_t status = 0b110011 << head;
//
//    for (int addr = 0; range; range <<= 2) {
//        while (status >> addr & 0x1) {
//            ++addr;
//        }
//
//        switch (range & M_11) {
//            case M_00: // space
//            case M_11: // 1x1
//                if (addr > 19) {
//                    return false;
//                }
//                status |= 0b1 << addr;
//                break;
//            case M_01: // 1x2
//                if (addr > 18 || (addr & 0b11) == 0b11 || status >> (addr + 1) & 0b1) {
//                    return false;
//                }
//                status |= 0b11 << addr;
//                break;
//            case M_10: // 2x1
//                if (addr > 15 || status >> (addr + 4) & 0b1) {
//                    return false;
//                }
//                status |= 0b10001 << addr;
//                break;
//        }
//    }
//
//    return true;

//    uint32_t status = 0x110011 << head;
//    for (int addr = 0; range; range >>= 2) {
//        while (status >> addr & 0x1) {
//            ++addr;
//        }
//        switch (range & 0x3) {
//            case 0b00: // space
//            case 0b11: // 1x1
//                if (addr > 19) {
//                    return false;
//                }
//                status |= 0b1 << addr;
//                break;
//            case 0b01: // 1x2
//                if (addr > 18 || addr % 4 == 3 || status >> (addr + 1) & 0x1) {
//                    return false;
//                }
//                status |= 0b11 << addr;
//                break;
//            case 0b10: // 2x1
//                if (addr > 15 || status >> (addr + 4) & 0x1) {
//                    return false;
//                }
//                status |= 0b10001 << addr;
//                break;
//        }
//    }
//    return true;


//}

int main() {

//    std::vector<uint32_t> temp;
//    load_basic_ranges(temp);
    // n1 = 2 | n2 = 1 | n3 = 3 | n4 = 6
//    build_basic_ranges(2, 1, 3, 6, temp);

    std::vector<uint64_t> temp;
    find_all_cases(temp);

    printf("size -> %ld\n", temp.size());

    return 0;
}
