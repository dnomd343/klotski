#include "all_cases.h"
#include "short_code.h"
#include "short_code_mark.h"

//#include <iostream>

void ShortCode::build_mapping() { // build fast search mapping
    auto all = AllCases(AllCases::InitType::WITH_ALL_CASES);
//    std::cout << "start build" << std::endl;
    for (int head = 0; head < 16; ++head) {
        uint64_t prefix = (uint64_t)head << 32;
        for (const auto &range : (*all.get_all_cases())[head]) {
            all_cases_list.emplace_back(prefix | range); // short_code => common_code
        }
    }
//    std::cout << "stage-1 ok" << std::endl;
    for (int n = 0; n < all_cases_list.size(); ++n) {
        all_cases_dict[all_cases_list[n]] = n; // common_code => short_code
    }
//    std::cout << "complete build" << std::endl;
}

void ShortCode::speed_up() {
    if (all_cases_list.empty()) {
        build_mapping();
    }
}
