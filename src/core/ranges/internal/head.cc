#include "utils/utility.h"
#include "ranges/ranges.h"

/// Check whether the combination of head and range is valid.
static int check_range(const int head, uint32_t range) noexcept {
    constexpr uint32_t M_1x1 = 0b00000001;
    constexpr uint32_t M_1x2 = 0b00000011;
    constexpr uint32_t M_2x1 = 0b00010001;
    constexpr uint32_t M_2x2 = 0b00110011;

    uint32_t flags = M_2x2 << head; // fill 2x2 block
    for (int addr = 0, offset = 1; range; range >>= 2, ++offset) { // traverse every 2-bit
        const auto num = klotski::low_zero_num(~flags);
        addr += num; // next unfilled block
        flags >>= num;
        switch (range & 0b11) {
            case 0b00: // space
            case 0b11: // 1x1 block
                flags |= M_1x1;
                continue;
            case 0b10: // 2x1 block
                if ((flags >> 4) & 0b1 || addr > 15) { // invalid case
                    return offset; // broken offset
                }
                flags |= M_2x1;
                continue;
            case 0b01: // 1x2 block
                if ((flags >> 1) & 0b1 || (addr & 0b11) == 0b11) { // invalid case
                    return offset; // broken offset
                }
                flags |= M_1x2;
                continue;
        }
    }
    return 0; // pass check
}

void klotski::cases::Ranges::with_head(const int head, Ranges &release) const {
    // release.clear();
    // release.reserve(ALL_CASES_NUM[head]);
    // auto &basic_ranges = BasicRanges::instance().fetch();

    for (uint32_t index = 0; index < ranges_.size(); ++index) {
        auto offset = check_range(head, ranges_[index]);
        if (offset) { // invalid case
            auto tmp = (uint32_t)0b1 << (32 - offset * 2); // distance to next possible range
            ///         !! <- broken
            /// ( xx xx xx ) xx xx xx ... [reversed range]
            ///         +1   00 00 00 ...     (delta)
            tmp += klotski::range_reverse(ranges_[index]) & ~(tmp - 1);
            while (klotski::range_reverse(ranges_[++index]) < tmp); // located next range
            --index;
            continue;
        }
        release.ranges_.emplace_back(klotski::range_reverse(ranges_[index])); // release valid case
    }
}
