#include <algorithm>
#include "common.h"
#include "short_code.h"
#include "all_cases_offset.h"
#include "basic_ranges_offset.h"
#include "range_prefix_offset.h"

using klotski::ShortCode;
using klotski::ShortCodes;
using klotski::CommonCode;

/// ------------------------- ShortCode to CommonCode -------------------------

CommonCode ShortCode::to_common_code() const noexcept {
    if (ShortCode::mode() == ShortCode::NORMAL) {
        return CommonCode::unsafe_create(tiny_decode(code_)); // normal mode
    }
    return CommonCode::unsafe_create(fast_decode(code_)); // fast mode
}

/// ------------------------- ShortCode to CommonCode -------------------------

ShortCode::ShortCode(CommonCode &&common_code) noexcept {
    if (ShortCode::mode() == ShortCode::NORMAL) {
        code_ = tiny_encode(common_code.unwrap()); // normal mode
    } else {
        code_ = fast_encode(common_code.unwrap()); // fast mode
    }
}

ShortCode::ShortCode(const CommonCode &common_code) noexcept {
    if (ShortCode::mode() == ShortCode::NORMAL) {
        code_ = tiny_encode(common_code.unwrap()); // normal mode
    } else {
        code_ = fast_encode(common_code.unwrap()); // fast mode
    }
}

ShortCode ShortCode::from_common_code(uint64_t common_code) {
    return ShortCode(CommonCode(common_code));
}

ShortCode ShortCode::from_common_code(CommonCode &&common_code) noexcept {
    return ShortCode(std::forward<CommonCode>(common_code));
}

ShortCode ShortCode::from_common_code(std::string &&common_code) {
    return ShortCode(std::forward<CommonCode>(
        CommonCode(std::forward<std::string>(common_code))
    ));
}

ShortCode ShortCode::from_common_code(const CommonCode &common_code) noexcept {
    return ShortCode(common_code);
}

ShortCode ShortCode::from_common_code(const std::string &common_code) {
    return ShortCode(CommonCode(common_code));
}

/// ---------------------------- Batch conversion -----------------------------

ShortCodes ShortCode::convert(const CommonCodes &common_codes) noexcept {
    return {common_codes.begin(), common_codes.end()};
}

/// ----------------------------- Basic Functions -----------------------------

/// NOTE: ensure that input common code is valid!
uint32_t ShortCode::fast_encode(uint64_t common_code) noexcept { // common code --> short code
    auto head = common_code >> 32; // head index
    const auto &ranges = AllCases::fetch()[head]; // available ranges
    auto offset = std::lower_bound(ranges.begin(), ranges.end(), (uint32_t)common_code) - ranges.begin();
    return ALL_CASES_OFFSET[head] + offset; // release short code
}

/// NOTE: ensure that input short code is valid!
uint64_t ShortCode::fast_decode(uint32_t short_code) noexcept { // short code --> common code
    auto offset = std::upper_bound( // using binary search
        ALL_CASES_OFFSET, ALL_CASES_OFFSET + 16, short_code
    ) - 1;
    uint64_t head = offset - ALL_CASES_OFFSET; // head index
    return (head << 32) | AllCases::fetch()[head][short_code - *offset]; // release common code
}

/// NOTE: ensure that input common code is valid!
uint32_t ShortCode::tiny_encode(uint64_t common_code) noexcept { // common code --> short code
    /// load head index and range prefix
    uint32_t head = common_code >> 32;
    uint32_t prefix = (common_code >> 20) & 0xFFF;

    /// search for target range
    uint32_t offset = 0;
    auto index = BASIC_RANGES_OFFSET[prefix];
    const auto &basic_ranges = BasicRanges::fetch();
    auto target = Common::range_reverse((uint32_t)common_code); // target range
    for (; index < basic_ranges.size(); ++index) {
        auto broken_offset = Common::check_range(head, basic_ranges[index]);
        if (!broken_offset) { // valid case
            if (basic_ranges[index] == target) {
                break; // found target range
            }
            ++offset; // record sub offset
        } else {
            auto delta = (uint32_t)1 << (32 - broken_offset * 2); // delta to next possible range
            auto next_min = (Common::range_reverse(basic_ranges[index]) & ~(delta - 1)) + delta;
            while (Common::range_reverse(basic_ranges[++index]) < next_min); // located next range
            --index;
        }
    }
    return ALL_CASES_OFFSET[head] + RANGE_PREFIX_OFFSET[head][prefix] + offset;
}

/// NOTE: ensure that input short code is valid!
uint64_t ShortCode::tiny_decode(uint32_t short_code) noexcept { // short code --> common code
    /// match head index
    auto offset = std::upper_bound( // binary search
        ALL_CASES_OFFSET, ALL_CASES_OFFSET + 16, short_code
    ) - 1;
    auto head = offset - ALL_CASES_OFFSET; // head index
    short_code -= *offset;

    /// match range prefix
    offset = std::upper_bound( // binary search
        RANGE_PREFIX_OFFSET[head], RANGE_PREFIX_OFFSET[head] + 4096, short_code
    ) - 1;
    auto prefix = offset - RANGE_PREFIX_OFFSET[head]; // range prefix
    short_code -= *offset;

    /// search for target range
    auto index = BASIC_RANGES_OFFSET[prefix];
    const auto &basic_ranges = BasicRanges::fetch();
    for (; index < basic_ranges.size(); ++index) { // traverse basic ranges
        auto broken_offset = Common::check_range(head, basic_ranges[index]);
        if (!broken_offset && !short_code--) { // valid case -> short code approximate
            break;
        }
        auto delta = (uint32_t)1 << (32 - broken_offset * 2); // delta to next possible range
        auto next_min = (Common::range_reverse(basic_ranges[index]) & ~(delta - 1)) + delta;
        while (Common::range_reverse(basic_ranges[++index]) < next_min); // located next range
        --index;
    }
    return (uint64_t)head << 32 | Common::range_reverse(basic_ranges[index]);
}
