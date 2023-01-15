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

#include <iostream>
#include <algorithm>

uint64_t ShortCode::fast_decode(uint32_t short_code) {
//    std::cout << "short code: " << short_code << std::endl;

    uint32_t head = std::upper_bound(ALL_CASES_OFFSET, ALL_CASES_OFFSET + 16, short_code) - ALL_CASES_OFFSET - 1;
//    std::cout << head << std::endl;

//    printf("%08X\n", AllCases::fetch()[head][ short_code - ALL_CASES_OFFSET[head] ]);

    return ((uint64_t)head << 32) | AllCases::fetch()[head][short_code - ALL_CASES_OFFSET[head]];
}

uint32_t ShortCode::fast_encode(uint64_t common_code) {
//    printf("common code: %09lX\n", common_code);

    uint32_t head = common_code >> 32;

//    std::cout << "head = " << head << std::endl;

    const auto &a = AllCases::fetch();

    return std::lower_bound(a[head].begin(), a[head].end(), (uint32_t)common_code) - a[head].begin() + ALL_CASES_OFFSET[head];

}
