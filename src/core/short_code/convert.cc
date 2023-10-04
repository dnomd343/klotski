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

/// NOTE: ensure that input common code is valid!
uint32_t ShortCode::tiny_encode(uint64_t common_code) noexcept { // common code --> short code
    /// load head index and range prefix
//    uint32_t head = common_code >> 32;
//    uint32_t prefix = (common_code >> 20) & 0xFFF;

    /// search for target range
//    uint32_t offset = 0;
//    auto index = BASIC_RANGES_OFFSET[prefix];
//    const auto &basic_ranges = BasicRanges::fetch();
//    auto target = Common::range_reverse((uint32_t)common_code); // target range
//    for (; index < basic_ranges.size(); ++index) {
//        auto broken_offset = Common::check_range(head, basic_ranges[index]);
//        if (!broken_offset) { // valid case
//            if (basic_ranges[index] == target) {
//                break; // found target range
//            }
//            ++offset; // record sub offset
//        } else {
//            auto delta = (uint32_t)1 << (32 - broken_offset * 2); // delta to next possible range
//            auto next_min = (Common::range_reverse(basic_ranges[index]) & ~(delta - 1)) + delta;
//            while (Common::range_reverse(basic_ranges[++index]) < next_min); // located next range
//            --index;
//        }
//    }
//    return ALL_CASES_OFFSET[head] + RANGE_PREFIX_OFFSET[head][prefix] + offset;
}

/// NOTE: ensure that input short code is valid!
uint64_t ShortCode::tiny_decode(uint32_t short_code) noexcept { // short code --> common code
    /// match head index
//    auto offset = std::upper_bound( // binary search
//        ALL_CASES_OFFSET, ALL_CASES_OFFSET + 16, short_code
//    ) - 1;
//    auto head = offset - ALL_CASES_OFFSET; // head index
//    short_code -= *offset;

    /// match range prefix
//    offset = std::upper_bound( // binary search
//        RANGE_PREFIX_OFFSET[head], RANGE_PREFIX_OFFSET[head] + 4096, short_code
//    ) - 1;
//    auto prefix = offset - RANGE_PREFIX_OFFSET[head]; // range prefix
//    short_code -= *offset;

    /// search for target range
//    auto index = BASIC_RANGES_OFFSET[prefix];
//    const auto &basic_ranges = BasicRanges::fetch();
//    for (; index < basic_ranges.size(); ++index) { // traverse basic ranges
//        auto broken_offset = Common::check_range(head, basic_ranges[index]);
//        if (!broken_offset && !short_code--) { // valid case -> short code approximate
//            break;
//        }
//        auto delta = (uint32_t)1 << (32 - broken_offset * 2); // delta to next possible range
//        auto next_min = (Common::range_reverse(basic_ranges[index]) & ~(delta - 1)) + delta;
//        while (Common::range_reverse(basic_ranges[++index]) < next_min); // located next range
//        --index;
//    }
//    return (uint64_t)head << 32 | Common::range_reverse(basic_ranges[index]);
}

} // namespace codec
} // namespace klotski
