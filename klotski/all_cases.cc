#include <algorithm>
#include "common.h"
#include "all_cases.h"

AllCases::AllCases(AllCases::Build type) { // class initialize
    build(type);
}

void AllCases::build(AllCases::Build type) { // build specific type
    switch (type) {
        case Build::BASIC_RANGES:
            get_basic_ranges(); // basic ranges initialize
            break;
        case Build::ALL_CASES:
            get_all_cases(); // all cases initialize
            break;
    }
}

const std::vector<uint32_t>* AllCases::get_basic_ranges() {
    if (basic_ranges.empty()) {
        build_basic_ranges(); // basic ranges initialize
    }
    return &basic_ranges;
}

const std::vector<uint32_t> (*AllCases::get_all_cases())[16] {
    if (all_cases->empty()) {
        find_all_cases(); // all cases initialize
    }
    return &all_cases;
}

inline uint32_t binary_count(uint32_t bin) { // get number of non-zero bits
    bin -= (bin >> 1) & 0x55555555;
    bin = (bin & 0x33333333) + ((bin >> 2) & 0x33333333);
    bin = ((bin >> 4) + bin) & 0x0F0F0F0F;
//    return (bin * 0x01010101) >> 24; // AMD CPU
    bin += bin >> 8;
    bin += bin >> 16;
    return bin & 0b111111;
}

void AllCases::find_all_cases() { // find all valid cases
    get_basic_ranges(); // ensure basic ranges exist
    for (uint32_t head = 0; head < 16; ++head) { // address of 2x2 block
        if ((head & 0b11) == 0b11) {
            continue; // invalid 2x2 address
        }
        for (uint32_t range : basic_ranges) { // check base on 2x2 address and range
            if (Common::check_case(head, range)) {
                all_cases[head].emplace_back(Common::range_reverse(range)); // found valid case
            }
        }
    }
}

void AllCases::build_basic_ranges() { // build basic ranges
    for (int n = 0; n <= 7; ++n) { // number of 1x2 and 2x1 block -> 0 ~ 7
        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) { // number of 2x1 block -> 0 ~ n
            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) { // number of 1x1 block -> 0 ~ (14 - 2n)
                int n_1x2 = n - n_2x1;
                int n_space = 16 - n * 2 - n_1x1;
                /// 0x0 -> 00 | 1x2 -> 01 | 2x1 -> 10 | 1x1 -> 11
                generate_ranges(n_space, n_1x2, n_2x1, n_1x1); // generate target ranges
            }
        }
    }
    std::sort(basic_ranges.begin(), basic_ranges.end()); // sort basic ranges
    for (uint32_t &range : basic_ranges) {
        range = Common::range_reverse(range); // range reverse
    }
}

void AllCases::generate_ranges(int n1, int n2, int n3, int n4) { // generate target ranges
    int len, limit;
    constexpr uint32_t M_01 = 0b01 << 30;
    constexpr uint32_t M_10 = 0b10 << 30;
    constexpr uint32_t M_11 = 0b11 << 30;
    std::vector<uint32_t> cache_1, cache_2;

    len = n1 + n2;
    limit = 0b1 << len;
    for (uint32_t bin = 0; bin < limit; ++bin) {
        if (binary_count(bin) != n2) { // skip binary without `n2` non-zero bits
            continue;
        }
        uint32_t range = 0;
        for (int i = 0; i < len; ++i) { // generate range base on binary value
            range >>= 2;
            if ((bin >> i) & 0b1) { // non-zero bit
                range |= M_01; // 01000...
            }
        }
        cache_1.emplace_back(range); // insert into first layer
    }

    len += n3;
    limit <<= n3;
    for (uint32_t bin = 0; bin < limit; ++bin) {
        if (binary_count(bin) != n3) { // skip binary without `n3` non-zero bits
            continue;
        }
        for (uint32_t base : cache_1) { // traverse first layer
            uint32_t range = 0;
            for (int i = 0; i < len; ++i) { // generate range base on binary value
                if ((bin >> i) & 0b1) { // non-zero bit
                    (range >>= 2) |= M_10; // 10000...
                    continue;
                }
                (range >>= 2) |= base & M_11;
                base <<= 2;
            }
            cache_2.emplace_back(range); // insert into second layer
        }
    }

    len += n4;
    limit <<= n4;
    for (uint32_t bin = 0; bin < limit; ++bin) {
        if (binary_count(bin) != n4) { // skip binary without `n4` non-zero bits
            continue;
        }
        for (uint32_t base : cache_2) { // traverse second layer
            uint32_t range = 0;
            for (int i = 0; i < len; ++i) { // generate range base on binary value
                if ((bin >> i) & 0b1) { // non-zero bit
                    (range >>= 2) |= M_11; // 11000...
                    continue;
                }
                (range >>= 2) |= base & M_11;
                base <<= 2;
            }
            basic_ranges.emplace_back(range); // insert into release ranges
        }
    }
}
