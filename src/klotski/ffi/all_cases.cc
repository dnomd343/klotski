#include "klotski.h"
#include "all_cases.h"

using klotski::AllCases;
using klotski::BasicRanges;
using klotski::BASIC_RANGES_SIZE;
using klotski::ALL_CASES_SIZE_SUM;

void all_cases_build() {
    AllCases::build();
}

void basic_ranges_build() {
    BasicRanges::build();
}

bool is_all_cases_available() {
    return AllCases::status() == AllCases::AVAILABLE;
}

bool is_basic_ranges_available() {
    return BasicRanges::status() == BasicRanges::AVAILABLE;
}
