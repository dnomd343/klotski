#include <algorithm>

#include "offset/offset.h"
#include "short_code/short_code.h"

using klotski::cases::AllCases;
using klotski::codec::ShortCode;
using klotski::cases::BasicRanges;

using klotski::cases::ALL_CASES_OFFSET;
using klotski::codec::RANGES_GLOBAL_OFFSET;
using klotski::codec::RANGES_SUBSET_OFFSET;

/// FIXME: temporarily used to implement tidy conversion
static uint32_t check_range(uint32_t head, uint32_t range) noexcept {
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

uint32_t ShortCode::tiny_encode(uint64_t common_code) {
    uint32_t head = common_code >> 32;
    uint32_t prefix = (common_code >> 20) & 0xFFF;

    uint32_t offset = 0;
    auto index = RANGES_GLOBAL_OFFSET[prefix];

    ranges_ = &cases::BasicRanges::instance().fetch();
    const auto &ranges = *ranges_;

    auto target = (uint32_t)common_code; // target range
    for (; index < ranges.size(); ++index) {
        auto broken_offset = check_range(head, range_reverse(ranges[index]));
        if (!broken_offset) { // valid case
            if (ranges[index] == target) {
                break; // found target range
            }
            ++offset; // record sub offset
        } else {
            auto delta = (uint32_t)1 << (32 - broken_offset * 2); // delta to next possible range
            auto next_min = (ranges[index] & ~(delta - 1)) + delta;
            while (ranges[++index] < next_min); // located next range
            --index;
        }
    }
    return ALL_CASES_OFFSET[head] + RANGES_SUBSET_OFFSET[head][prefix] + offset;
}

uint64_t ShortCode::tiny_decode(uint32_t short_code) { // short code --> common code
    auto offset_ = std::upper_bound(ALL_CASES_OFFSET.begin(), ALL_CASES_OFFSET.end(), short_code) - 1;
    auto head = offset_ - ALL_CASES_OFFSET.begin(); // head index
    short_code -= *offset_;

    auto offset = std::upper_bound(RANGES_SUBSET_OFFSET[head].begin(), RANGES_SUBSET_OFFSET[head].end(), short_code) - 1;
    auto prefix = offset - RANGES_SUBSET_OFFSET[head].begin(); // range prefix
    short_code -= *offset;

    /// search for target range
    auto index = RANGES_GLOBAL_OFFSET[prefix];

    ranges_ = &cases::BasicRanges::instance().fetch();
    const auto &ranges = *ranges_;

    for (; index < ranges.size(); ++index) { // traverse basic ranges
        auto broken_offset = check_range(head, range_reverse(ranges[index]));
        if (!broken_offset && !short_code--) { // valid case -> short code approximate
            break;
        }
        auto delta = (uint32_t)1 << (32 - broken_offset * 2); // delta to next possible range
        auto next_min = (ranges[index] & ~(delta - 1)) + delta;
        while (ranges[++index] < next_min); // located next range
        --index;
    }
    return (uint64_t)head << 32 | ranges[index];
}
