#include "common.h"
#include "short_code.h"
#include "basic_ranges.h"
#include "short_code_mark.h"

CommonCode ShortCode::to_common_code() const { // convert to common code
    if (ShortCode::check_mode() == ShortCode::NORMAL) {
        return CommonCode::unsafe_create(tiny_decode(code)); // using normal mode
    }
    return CommonCode::unsafe_create(all_cases_list[code]); // using fast mode
}

ShortCode::ShortCode(const CommonCode &common_code) { // convert from common code
    if (ShortCode::check_mode() == ShortCode::NORMAL) {
        code = tiny_encode(common_code.unwrap()); // using normal mode
    } else {
        code = all_cases_dict[common_code.unwrap()]; // using fast mode
    }
}

/// ensure that input common code is valid
uint32_t ShortCode::tiny_encode(uint64_t common_code) { // common_code --low-memory--> short_code
    uint32_t offset = 0;
    uint32_t head = common_code >> 32; // common code head
    uint32_t prefix = (common_code >> 24) & 0xFF; // common code range prefix
    auto target = Common::range_reverse((uint32_t)common_code); // target range

    for (int index = 0; index < BASIC_RANGES_INDEX[prefix]; ++index) { // traverse basic ranges
        uint32_t range = (*BasicRanges::fetch())[index + BASIC_RANGES_OFFSET[prefix]];
        if (range == target) {
            break; // found target range
        }
        if (Common::check_case(head, range)) { // search for valid cases
            ++offset; // record sub offset
        }
    }
    return ALL_CASES_OFFSET[head] + RANGE_PREFIX_OFFSET[head][prefix] + offset;
}

/// ensure that input short code is valid
uint64_t ShortCode::tiny_decode(uint32_t short_code) { // short_code --low-memory--> common_code
    uint32_t head = 0, prefix = 0;
    for (; head < 16; ++head) {
        if (short_code < ALL_CASES_INDEX[head]) { // match head
            break;
        }
        short_code -= ALL_CASES_INDEX[head]; // short code approximate
    }
    for (; prefix < 256; ++prefix) {
        if (short_code < RANGE_PREFIX_INDEX[head][prefix]) { // match range prefix
            break;
        }
        short_code -= RANGE_PREFIX_INDEX[head][prefix]; // short code approximate
    }

    uint32_t range;
    for (int index = 0; index < BASIC_RANGES_INDEX[prefix]; ++index) { // traverse basic ranges
        range = (*BasicRanges::fetch())[index + BASIC_RANGES_OFFSET[prefix]];
        if (Common::check_case(head, range)) { // search for valid cases
            if (short_code == 0) {
                break; // found target range
            }
            --short_code; // short code approximate
        }
    }
    return (uint64_t)head << 32 | Common::range_reverse(range); // release common code
}
