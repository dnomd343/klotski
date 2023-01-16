#include <algorithm>
#include "common.h"
#include "short_code.h"
#include "basic_ranges.h"

//#include "short_code_mark.h"

CommonCode ShortCode::to_common_code() const { // convert to common code
    if (ShortCode::check_mode() == ShortCode::NORMAL) {
        return CommonCode::unsafe_create(tiny_decode(code)); // normal mode
    }
    return CommonCode::unsafe_create(fast_decode(code)); // fast mode
}

ShortCode::ShortCode(const CommonCode &common_code) { // convert from common code
    if (ShortCode::check_mode() == ShortCode::NORMAL) {
        code = tiny_encode(common_code.unwrap()); // normal mode
    } else {
        code = fast_encode(common_code.unwrap()); // fast mode
    }
}

#include "all_cases.h"
#include "all_cases_offset.h"
#include "basic_ranges_offset.h"
#include "range_prefix_offset.h"

uint64_t ShortCode::fast_decode(uint32_t short_code) { // short code => common code
    auto offset = std::upper_bound( // using binary search
        ALL_CASES_OFFSET, ALL_CASES_OFFSET + 16, short_code
    ) - 1;
    uint64_t head = offset - ALL_CASES_OFFSET; // head index
    return (head << 32) | AllCases::fetch()[head][short_code - *offset]; // release common code
}

uint32_t ShortCode::fast_encode(uint64_t common_code) { // common code => short code
    auto head = common_code >> 32; // head index
    const auto &ranges = AllCases::fetch()[head]; // available ranges
    auto offset = std::lower_bound(ranges.begin(), ranges.end(), (uint32_t)common_code) - ranges.begin();
    return ALL_CASES_OFFSET[head] + offset; // release short code
}

/// ensure that input common code is valid
//uint32_t ShortCode::tiny_encode(uint64_t common_code) { // common_code --low-memory--> short_code
//    uint32_t offset = 0;
//    uint32_t head = common_code >> 32; // common code head
//    uint32_t prefix = (common_code >> 24) & 0xFF; // common code range prefix
//    auto target = Common::range_reverse((uint32_t)common_code); // target range
//
//    for (int index = 0; index < BASIC_RANGES_INDEX[prefix]; ++index) { // traverse basic ranges
//        uint32_t range = BasicRanges::fetch()[index + BASIC_RANGES_OFFSET[prefix]];
//        if (range == target) {
//            break; // found target range
//        }
//        if (Common::check_case(head, range)) { // search for valid cases
//            ++offset; // record sub offset
//        }
//    }
//    return ALL_CASES_OFFSET[head] + RANGE_PREFIX_OFFSET[head][prefix] + offset;
//}

/// ensure that input short code is valid
//uint64_t ShortCode::tiny_decode(uint32_t short_code) { // short_code --low-memory--> common_code
//    uint32_t head = 0, prefix = 0;
//    for (; head < 16; ++head) {
//        if (short_code < ALL_CASES_INDEX[head]) { // match head
//            break;
//        }
//        short_code -= ALL_CASES_INDEX[head]; // short code approximate
//    }
//    for (; prefix < 256; ++prefix) {
//        if (short_code < RANGE_PREFIX_INDEX[head][prefix]) { // match range prefix
//            break;
//        }
//        short_code -= RANGE_PREFIX_INDEX[head][prefix]; // short code approximate
//    }
//
//    uint32_t range;
//    for (int index = 0; index < BASIC_RANGES_INDEX[prefix]; ++index) { // traverse basic ranges
//        range = BasicRanges::fetch()[index + BASIC_RANGES_OFFSET[prefix]];
//        if (Common::check_case(head, range)) { // search for valid cases
//            if (short_code == 0) {
//                break; // found target range
//            }
//            --short_code; // short code approximate
//        }
//    }
//    return (uint64_t)head << 32 | Common::range_reverse(range); // release common code
//}

// TODO: try to using 10-bits range prefix -> less static data

uint64_t ShortCode::tiny_decode(uint32_t short_code) {
    /// match head index
    auto offset = std::upper_bound( // binary search
            ALL_CASES_OFFSET, ALL_CASES_OFFSET + 16, short_code
    ) - 1;
    uint64_t head = offset - ALL_CASES_OFFSET; // head index
    short_code -= *offset;

    /// match range prefix
    offset = std::upper_bound( // binary search
            RANGE_PREFIX_OFFSET[head], RANGE_PREFIX_OFFSET[head] + 4096, short_code
    ) - 1;
    uint32_t prefix = offset - RANGE_PREFIX_OFFSET[head]; // range prefix
    short_code -= *offset;

    /// search target range
    const auto &basic_ranges = BasicRanges::fetch();
    for (auto index = BASIC_RANGES_OFFSET[prefix]; index < basic_ranges.size(); ++index) {

        uint32_t range = basic_ranges[index]; // traverse basic ranges

        uint32_t broken = Common::check_range(head, basic_ranges[index]); // check and get broken address

        auto range_rev = Common::range_reverse(basic_ranges[index]); // reversed range

        if (broken) { // invalid case
            auto delta = (uint32_t)1 << (32 - broken * 2); // this --delta--> next possible range
            auto next_min = (range_rev & ~(delta - 1)) + delta;
            while (Common::range_reverse(basic_ranges[++index]) < next_min); // located next range
            --index;
        } else {

            if (!short_code--) { // short code approximate

                /// found target range
                return head << 32 | range_rev;

            }

        }

    }

    return 0; // never reach when input valid
}

uint32_t ShortCode::tiny_encode(uint64_t common_code) {

//    printf("%09lX\n", common_code);

    uint32_t head = common_code >> 32;
    uint32_t prefix = (common_code >> 20) & 0xFFF;

//    printf("head = %d\n", head);
//    printf("prefix = %X\n", prefix);

    uint32_t offset = 0;
    auto target = Common::range_reverse((uint32_t)common_code); // target range

    const auto &basic_ranges = BasicRanges::fetch();

    for (auto index = BASIC_RANGES_OFFSET[prefix]; index < basic_ranges.size(); ++index) { // traverse basic ranges

        uint32_t range = basic_ranges[index];

        if (Common::check_case(head, range)) { // search for valid cases

            if (range == target) { // found target range
                return ALL_CASES_OFFSET[head] + RANGE_PREFIX_OFFSET[head][prefix] + offset;

            }

            ++offset; // record sub offset

        }

    }


    return 0;
}

