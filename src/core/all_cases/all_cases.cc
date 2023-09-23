#include <iostream>

#include "reverse.h"
#include "all_cases.h"

//typedef uint32_t Range;

using klotski::cases::BasicRanges;
using klotski::cases::range_reverse;

inline int low_zero_num(uint32_t bin) {
    return __builtin_ctzl(bin);

    // WARN: be aware of serious performance issues
    // return __builtin_popcount(~(bin ^ -bin)) - 1;
}

int check_range(int head, uint32_t range) noexcept {
    constexpr uint32_t MASK_1x1 = 0b00000001;
    constexpr uint32_t MASK_1x2 = 0b00000011;
    constexpr uint32_t MASK_2x1 = 0b00010001;
    constexpr uint32_t MASK_2x2 = 0b00110011;

    uint32_t flags = MASK_2x2 << head; // fill 2x2 block
    for (int addr = 0, offset = 1; range; range >>= 2, ++offset) { // traverse every 2-bit
        auto num = low_zero_num(~flags);
        addr += num; // next unfilled block
        flags >>= num;
        switch (range & 0b11) {
            case 0b00: // space
            case 0b11: // 1x1 block
                flags |= MASK_1x1;
                continue;
            case 0b10: // 2x1 block
                if ((flags >> 4) & 0b1 || addr > 15) { // invalid case
                    return offset; // broken offset
                }
                flags |= MASK_2x1;
                continue;
            case 0b01: // 1x2 block
                if ((flags >> 1) & 0b1 || (addr & 0b11) == 0b11) { // invalid case
                    return offset; // broken offset
                }
                flags |= MASK_1x2;
                continue;
        }
    }
    return 0; // pass check
}

void demo() {

//    std::cout << __builtin_popcount(~(6 ^ -6)) - 1 << std::endl;
//    return;

    constexpr std::array<int, 12> heads = {
        0x0, 0x1, 0x2,
        0x4, 0x5, 0x6,
        0x8, 0x9, 0xA,
        0xC, 0xD, 0xE,
    };

    auto num = 0;

    std::array<std::vector<uint32_t>, 16> data;

    // TODO: vector reserve

    auto basic_ranges = BasicRanges::Instance().Fetch();

//    std::vector<uint32_t> reversed {basic_ranges.begin(), basic_ranges.end()};
//    for (auto &x : reversed) {
//        x = range_reverse(x);
//    }

    for (auto head : heads) {
        for (uint32_t index = 0; index < basic_ranges.size(); ++index) {
            auto offset = check_range(head, basic_ranges[index]);
            if (offset) { // invalid case
                auto tmp = (uint32_t)0b1 << (32 - offset * 2); // distance to next possible range
                ///         !! <- broken
                /// ( xx xx xx ) xx xx xx ... [reversed range]
                ///         +1   00 00 00 ...     (delta)
                tmp += range_reverse(basic_ranges[index]) & ~(tmp - 1);
                while (range_reverse(basic_ranges[++index]) < tmp); // located next range
                --index;

//                next += reversed[index] & ~(next - 1);
//                auto kk = std::lower_bound(reversed.begin() + index, reversed.end(), next);
//                index = kk - reversed.begin() - 1;

                continue;
            }

            data[head].emplace_back(range_reverse(basic_ranges[index]));

//            ++index;

            ++num;
//            volatile auto r = range_reverse(basic_ranges[index]); // release valid cases
        }
    }

//    for (auto &x : data) {
//        std::cout << x.size() << std::endl;
//    }

    for (uint64_t head = 0; head < 15; ++head) {
        for (auto x : data[head]) {
            printf("%09llX\n", (head << 32) | x);
        }
    }

//    std::cout << num << std::endl;

}
