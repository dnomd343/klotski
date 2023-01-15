#include "all_cases.h"
#include "short_code.h"
#include "basic_ranges.h"
#include "short_code_mark.h"

std::mutex ShortCode::map_building;
bool ShortCode::fast_mode_available = false;
bool ShortCode::normal_mode_available = false;

std::vector<uint64_t> ShortCode::all_cases_list;
std::unordered_map<uint64_t, uint32_t> ShortCode::all_cases_dict;

void ShortCode::speed_up(ShortCode::Mode mode) {
    if (fast_mode_available) {
        return; // fast mode already available
    }
    if (mode == ShortCode::FAST) { // build fast mode data
        build_mappings();
    } else if (mode == ShortCode::NORMAL && !normal_mode_available) { // build normal mode data
        BasicRanges::build(); // blocking function
        normal_mode_available = true;
    }
}

ShortCode::Mode ShortCode::check_mode() { // ensure speed up enabled and return current mode
    if (fast_mode_available) {
        return ShortCode::FAST; // fast mode already enabled
    }
    if (normal_mode_available) {
        return ShortCode::NORMAL; // normal mode already enabled
    }
    speed_up(ShortCode::Mode::NORMAL); // without initialized -> enter normal mode
    return ShortCode::Mode::NORMAL; // use normal mode
}

/// ensure that fast_mode_available == false
void ShortCode::build_mappings() { // build fast search mappings
    if (map_building.try_lock()) { // lock success -> start building
        for (int head = 0; head < 16; ++head) {
            uint64_t prefix = (uint64_t)head << 32;
            for (const auto &range : AllCases::fetch()[head]) { // blocking function
                all_cases_list.emplace_back(prefix | range); // short_code -> common_code
            }
        }
        for (int index = 0; index < all_cases_list.size(); ++index) {
            all_cases_dict[all_cases_list[index]] = index; // common_code -> short_code
        }
        fast_mode_available = true; // set available flag
    } else { // another thread building
        map_building.lock(); // blocking waiting
    }
    map_building.unlock();
}

#include <algorithm>

uint64_t ShortCode::fast_decode(uint32_t short_code) {
    auto offset = std::upper_bound(ALL_CASES_OFFSET, ALL_CASES_OFFSET + 16, short_code) - 1; // binary search
    uint64_t head = offset - ALL_CASES_OFFSET; // head index
    return (head << 32) | AllCases::fetch()[head][short_code - *offset]; // release common code
}

uint32_t ShortCode::fast_encode(uint64_t common_code) {
    auto head = common_code >> 32; // head index
    const auto &ranges = AllCases::fetch()[head]; // available ranges
    auto offset = std::lower_bound(ranges.begin(), ranges.end(), (uint32_t)common_code) - ranges.begin();
    return ALL_CASES_OFFSET[head] + offset; // release short code
}

#include "basic_ranges_offset.h"
#include "range_prefix_offset.h"

#include <iostream>
#include "common.h"

uint64_t ShortCode::tiny_decode_demo(uint32_t short_code) {
    /// match head index
    auto offset = std::upper_bound( // binary search
        ALL_CASES_OFFSET, ALL_CASES_OFFSET + 16, short_code
    ) - 1;
    uint64_t head = offset - ALL_CASES_OFFSET; // head index
    short_code -= *offset;

    /// match range prefix
    offset = std::upper_bound( // binary search
        RANGE_PREFIX_OFFSET_[head], RANGE_PREFIX_OFFSET_[head] + 4096, short_code
    ) - 1;
    uint32_t prefix = offset - RANGE_PREFIX_OFFSET_[head]; // range prefix
    short_code -= *offset;

    uint32_t start_point = BASIC_RANGES_OFFSET_[prefix];

    printf("prefix: %X\n", prefix);
    printf("start at %d\n", start_point);
    printf("next %d valid case\n", short_code);

    uint32_t range;

    for (auto index = start_point; index < BasicRanges::fetch().size(); ++index) { // traverse basic ranges
        range = BasicRanges::fetch()[index];
        if (Common::check_case(head, range)) { // search for valid cases

            if (!short_code--) { // short code approximate
                break; // found target range
            }

        }
    }

    printf("range -> %08X\n", Common::range_reverse(range));

    return (uint64_t)head << 32 | Common::range_reverse(range);

}
