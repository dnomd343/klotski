#include "all_cases.h"

std::vector<uint32_t> AllCases::basic_ranges;

void AllCases::build_basic_ranges() {
    AllCases::basic_ranges.emplace_back(0);
    AllCases::basic_ranges.emplace_back(1);
    AllCases::basic_ranges.emplace_back(2);
}
