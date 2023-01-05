#include "all_cases.h"
#include "short_code.h"
#include "short_code_mark.h"

void ShortCode::build_base_ranges() {
    auto all = AllCases(AllCases::InitType::WITH_BASIC_RANGES);
    basic_ranges = *all.get_basic_ranges();
}

void ShortCode::build_mappings() { // build fast search mapping
    auto all = AllCases(AllCases::InitType::WITH_ALL_CASES);
    for (int head = 0; head < 16; ++head) {
        uint64_t prefix = (uint64_t)head << 32;
        for (const auto &range : (*all.get_all_cases())[head]) {
            all_cases_list.emplace_back(prefix | range); // short_code => common_code
        }
    }
    for (int n = 0; n < all_cases_list.size(); ++n) {
        all_cases_dict[all_cases_list[n]] = n; // common_code => short_code
    }
}

void ShortCode::speed_up(enum Mode mode) { // speed up handle short code
    switch (mode) {
        case Mode::NORMAL: // speed up into normal mode
            if (basic_ranges.empty()) {
                build_base_ranges(); // basic ranges initialize
            }
            break;
        case Mode::FAST: // speed up into fast mode
            if (all_cases_list.empty()) {
                build_mappings(); // all cases mapping initialize
            }
            break;
    }
}

uint64_t ShortCode::fast_decode(uint32_t short_code) { // short_code -> common_code
    // TODO: ensure input short_code < SHORT_CODE_LIMIT
    return all_cases_list[short_code];
}

uint32_t ShortCode::fast_encode(uint64_t common_code) { // common_code -> short_code
    // TODO: ensure input common_code valid
    return all_cases_dict[common_code];
}
