#include <algorithm>
#include "short_code.h"
#include "offset/basic.h"
#include "offset/range_prefix.h"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::codec::offset::ALL_CASES_OFFSET;
using klotski::codec::offset::BASIC_RANGES_OFFSET;
using klotski::codec::offset::RANGE_PREFIX_OFFSET;

namespace klotski {
namespace codec {

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

/// Convert CommonCode to ShortCode based on AllCases data.
uint32_t ShortCode::fast_encode(uint64_t common_code) noexcept {
    auto head = common_code >> 32;
    auto &ranges = AllCases::instance().fetch()[head]; // match available ranges
    auto target = std::lower_bound(ranges.begin(), ranges.end(), (uint32_t)common_code);
    return ALL_CASES_OFFSET[head] + (target - ranges.begin());
}

/// Convert ShortCode to CommonCode based on AllCases data.
uint64_t ShortCode::fast_decode(uint32_t short_code) noexcept {
    auto offset = std::upper_bound(ALL_CASES_OFFSET, ALL_CASES_OFFSET + 16, short_code) - 1;
    uint64_t head = offset - ALL_CASES_OFFSET;
    return (head << 32) | AllCases::instance().fetch()[head][short_code - *offset];
}

/// Convert CommonCode to ShortCode based on BasicRanges data.
uint32_t ShortCode::tiny_encode(uint64_t common_code) noexcept {
    uint32_t head = common_code >> 32;
    uint32_t prefix = (common_code >> 20) & 0xFFF;

    uint32_t offset = 0;
    auto index = BASIC_RANGES_OFFSET[prefix];
    const auto &basic_ranges = BasicRanges::instance().fetch();
    auto target = range_reverse((uint32_t)common_code); // target range
    for (; index < basic_ranges.size(); ++index) {
        auto broken_offset = check_range(head, basic_ranges[index]);
        if (!broken_offset) { // valid case
            if (basic_ranges[index] == target) {
                break; // found target range
            }
            ++offset; // record sub offset
        } else {
            auto delta = (uint32_t)1 << (32 - broken_offset * 2); // delta to next possible range
            auto next_min = (range_reverse(basic_ranges[index]) & ~(delta - 1)) + delta;
            while (range_reverse(basic_ranges[++index]) < next_min); // located next range
            --index;
        }
    }
    return ALL_CASES_OFFSET[head] + RANGE_PREFIX_OFFSET[head][prefix] + offset;
}

/// NOTE: ensure that input short code is valid!
uint64_t ShortCode::tiny_decode(uint32_t short_code) noexcept { // short code --> common code
    auto offset = std::upper_bound(ALL_CASES_OFFSET, ALL_CASES_OFFSET + 16, short_code) - 1;
    auto head = offset - ALL_CASES_OFFSET; // head index
    short_code -= *offset;

    offset = std::upper_bound(RANGE_PREFIX_OFFSET[head], RANGE_PREFIX_OFFSET[head] + 4096, short_code) - 1;
    auto prefix = offset - RANGE_PREFIX_OFFSET[head]; // range prefix
    short_code -= *offset;

    /// search for target range
    auto index = BASIC_RANGES_OFFSET[prefix];
    const auto &basic_ranges = BasicRanges::instance().fetch();
    for (; index < basic_ranges.size(); ++index) { // traverse basic ranges
        auto broken_offset = check_range(head, basic_ranges[index]);
        if (!broken_offset && !short_code--) { // valid case -> short code approximate
            break;
        }
        auto delta = (uint32_t)1 << (32 - broken_offset * 2); // delta to next possible range
        auto next_min = (range_reverse(basic_ranges[index]) & ~(delta - 1)) + delta;
        while (range_reverse(basic_ranges[++index]) < next_min); // located next range
        --index;
    }
    return (uint64_t)head << 32 | range_reverse(basic_ranges[index]);
}

} // namespace codec
} // namespace klotski
