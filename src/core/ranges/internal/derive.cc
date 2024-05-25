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

void Ranges::derive(const int head, Ranges &output) const {
    for (uint32_t index = 0; index < size(); ++index) {
        if (const auto offset = check_range(head, (*this)[index])) { // invalid case
            uint32_t tmp = 1U << (32 - offset * 2); // distance to next possible range
            ///         !! <- broken
            /// ( xx xx xx ) xx xx xx ... [reversed range]
            ///         +1   00 00 00 ...     (delta)
            tmp += range_reverse((*this)[index]) & ~(tmp - 1);
            while (range_reverse((*this)[++index]) < tmp) {} // located next range
            --index;
            continue;
        }
        output.emplace_back(range_reverse((*this)[index])); // release valid case
    }
}
