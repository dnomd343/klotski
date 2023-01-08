#include "all_cases.h"
#include "short_code.h"
#include "basic_ranges.h"

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

        // build_mappings
        build_mappings();

    } else if (mode == ShortCode::NORMAL && !normal_mode_available) { // build normal mode data
        BasicRanges::build_basic_ranges(); // blocking function
        normal_mode_available = true;
    }

}

void ShortCode::build_mappings() { // build fast search mappings

    if (fast_mode_available) {
        return; // all cases map already built
    }

    if (map_building.try_lock()) { // lock success -> start building

        AllCases::build_all_cases(); // blocking function

        for (int head = 0; head < 16; ++head) {
            uint64_t prefix = (uint64_t)head << 32;
            for (const auto &range : (*AllCases::get_all_cases())[head]) {
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
