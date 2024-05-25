#include "utils/utility.h"
#include "ranges/ranges.h"

#include "all_cases/all_cases.h"

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

void klotski::cases::global_derive(const std::vector<uint32_t> &range, std::vector<uint32_t> &output, int head) {

    for (uint32_t index = 0; index < range.size(); ++index) {
        if (const auto offset = check_range(head, range[index])) { // invalid case

            // if (offset > 14) {
            //     continue;
            // }

            int left_offset = (16 - offset) * 2;
            uint32_t min_next = ((range_reverse(range[index]) >> left_offset) + 1) << left_offset;

            // if (offset > 5) {
            while (range_reverse(range[++index]) < min_next) {} // located next range
            --index;
            // } else {
            //     auto begin = reversed.begin() + index;
            //     auto kk = std::lower_bound(begin, reversed.end(), min_next) - begin;
            //     index += kk - 1;
            // }

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

void klotski::cases::global_derive_pro(const std::vector<uint32_t> &range, const std::vector<uint32_t> &reversed, std::vector<uint32_t> &output, int head) {

    // uint32_t reversed_max = reversed.back();
    for (uint32_t index = 0; index < range.size(); ++index) {
        if (const auto offset = check_range(head, range[index])) { // invalid case

            if (offset > 14) {
                continue;
            }

            int left_offset = (16 - offset) * 2;
            uint32_t min_next = ((reversed[index] >> left_offset) + 1) << left_offset;

            if (offset > 5) {
                while (reversed[++index] < min_next) {} // located next range
                --index;
            } else {
                auto begin = reversed.begin() + index;
                auto kk = std::lower_bound(begin, reversed.end(), min_next) - begin;
                index += kk - 1;
            }

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
