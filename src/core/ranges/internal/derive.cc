#include <iostream>

#include <format>

#include "utils/utility.h"
#include "ranges/ranges.h"

using klotski::cases::Ranges;

/// Check whether the combination of head and range is valid.
static int check_range(const int head, uint32_t range) {
    uint32_t flags = 0b110011 << head; // fill 2x2 block
    for (int addr = 0, offset = 1; range; range >>= 2, ++offset) { // traverse every 2-bit
        const auto num = std::countr_one(flags);
        addr += num; // next unfilled block
        flags >>= num;
        switch (range & 0b11) {
            case 0b00: // space
            case 0b11: // 1x1 block
                flags |= 0b1;
                continue;
            case 0b01: // 1x2 block
                if (flags & 0b10 || addr % 4 == 3) { // invalid case
                    return offset; // broken offset
                }
                flags |= 0b11;
                continue;
            case 0b10: // 2x1 block
                if (flags & 0b10000 || addr > 15) { // invalid case
                    return offset; // broken offset
                }
                flags |= 0b10001;
        }
    }
    return 0; // pass check
}

// 0: [7, 8, 9, 10, 11, 12, 13, 14, 15]
// 1: [9, 10, 11, 12, 13, 14]
// 2: [8, 9, 10, 11, 12, 13]
// 3: [7, 8, 9, 10, 11, 12]
// 5: [8, 9, 10, 11, 12, 13]
// 10: [7, 8, 9, 10, 11, 12]
// 15: [8, 9, 10, 11, 12]

// 190: [5, 6, 7, 8]
// 284: [6, 7, 8]
// 327: [6, 7, 8]
// 591: [5, 6, 7, 8]
// 810: [5, 6, 7, 8]
// 895: [6, 7, 8]
// 1784: [4, 5, 6, 7]
// 2276: [5, 6, 7]
// 2447: [5, 6, 7]
// 5245: [4, 5, 6]
// 6346: [4, 5, 6]
// 6687: [5, 6]
// 15162: [3, 4, 5]
// 17588: [4, 5]
// 18271: [4, 5]
// 43243: [3, 4]
// 48548: [3]
// 48554: [3, 4]
// 49919: [4]
// 122103: [2]
// 122124: [2, 3]
// 133652: [3]
// 136382: [3]
// 342265: [2]
// 367139: [2]

void klotski::cases::derive_demo(const std::vector<uint32_t> &range, const std::vector<uint32_t> &reversed, std::vector<uint32_t> &output, int head) {

    uint32_t reversed_max = reversed.back();
    for (uint32_t index = 0; index < range.size(); ++index) {
        if (const auto offset = check_range(head, range[index])) { // invalid case

            if (offset > 14) {
                continue;
            }

            // uint32_t index_bak = index;

            int left_offset = (16 - offset) * 2;
            uint32_t min_next = ((reversed[index] >> left_offset) + 1) << left_offset;
            // min_next = std::min(min_next, reversed_max);
            // std::cout << min_next << " vs " << reversed_max << std::endl;

            if (offset > 5) {
                while (reversed[++index] < min_next) { // located next range
                    // if (index > range.size()) {
                    //     std::cout << "get it" << std::endl;
                    // }
                }
                --index;
            } else {
                auto begin = reversed.begin() + index;
                auto kk = std::lower_bound(begin, reversed.end(), min_next) - begin;
                index += kk - 1;
            }

            // std::cout << index << " vs " << index_bak << std::endl;

            // auto end_distance = range.size() - index;
            // auto real_distance = index - index_bak;

            // std::cout << std::format("{} {} {}", head, real_distance, offset) << std::endl;

            // std::cout << real_distance << " vs " << end_distance << std::endl;

            // uint32_t tmp = 1U << (32 - offset * 2); // distance to next possible range
            ///         !! <- broken
            /// ( xx xx xx ) xx xx xx ... [reversed range]
            ///         +1   00 00 00 ...     (delta)
            // tmp += range_reverse(range[index]) & ~(tmp - 1);

            // TODO: overflow here in some type_id
            // TODO:   -> tmp > range[-1]
            // TODO: maybe using binary search here

            // while (range_reverse(range[++index]) < tmp) {} // located next range
            // --index;
            continue;
        }
        output.emplace_back(range_reverse(range[index])); // release valid case
    }
}

void klotski::cases::derive_demo_pro(const klotski::cases::BidiRanges &bidi_range, std::vector<uint32_t> &output, int head) {
    for (uint32_t index = 0; index < bidi_range.size(); ++index) {
        if (const auto offset = check_range(head, bidi_range[index].r1)) { // invalid case

            // if (offset > 14) {
            //     continue;
            // }

            // uint32_t index_bak = index;

            int left_offset = (16 - offset) * 2;
            uint32_t min_next = ((bidi_range[index].r2 >> left_offset) + 1) << left_offset;
            // min_next = std::min(min_next, reversed_max);
            // std::cout << min_next << " vs " << reversed_max << std::endl;

            // if (offset > 5) {
                while (bidi_range[++index].r2 < min_next) { // located next range
                    // if (index > range.size()) {
                    //     std::cout << "get it" << std::endl;
                    // }
                }
                --index;
            // } else {
            //     auto begin = reversed.begin() + index;
            //     auto kk = std::lower_bound(begin, reversed.end(), min_next) - begin;
            //     index += kk - 1;
            // }

            // std::cout << index << " vs " << index_bak << std::endl;

            // auto end_distance = range.size() - index;
            // auto real_distance = index - index_bak;

            // std::cout << std::format("{} {} {}", head, real_distance, offset) << std::endl;

            // std::cout << real_distance << " vs " << end_distance << std::endl;

            // uint32_t tmp = 1U << (32 - offset * 2); // distance to next possible range
            ///         !! <- broken
            /// ( xx xx xx ) xx xx xx ... [reversed range]
            ///         +1   00 00 00 ...     (delta)
            // tmp += range_reverse(range[index]) & ~(tmp - 1);

            // TODO: overflow here in some type_id
            // TODO:   -> tmp > range[-1]
            // TODO: maybe using binary search here

            // while (range_reverse(range[++index]) < tmp) {} // located next range
            // --index;
            continue;
        }
        output.emplace_back(bidi_range[index].r2); // release valid case
    }
}

void Ranges::derive(const int head, Ranges &output) const {
    for (uint32_t index = 0; index < size(); ++index) {
        if (const auto offset = check_range(head, (*this)[index])) { // invalid case
            uint32_t tmp = 1U << (32 - offset * 2); // distance to next possible range
            ///         !! <- broken
            /// ( xx xx xx ) xx xx xx ... [reversed range]
            ///         +1   00 00 00 ...     (delta)
            tmp += range_reverse((*this)[index]) & ~(tmp - 1);

            // TODO: overflow here in some type_id
            // TODO:   -> tmp > range[-1]
            // TODO: maybe using binary search here

            // while (range_reverse((*this)[++index]) < tmp) {} // located next range
            // --index;
            continue;
        }
        output.emplace_back(range_reverse((*this)[index])); // release valid case
    }
}
