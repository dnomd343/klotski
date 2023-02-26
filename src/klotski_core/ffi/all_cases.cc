#include <cstring>
#include "klotski.h"
#include "all_cases.h"

using klotski::AllCases;
using klotski::BasicRanges;

const uint32_t ALL_CASES_SIZE = klotski::ALL_CASES_SIZE_SUM;
const uint32_t BASIC_RANGES_SIZE = klotski::BASIC_RANGES_SIZE;

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

void export_all_cases(uint64_t *buffer) {
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : AllCases::fetch()[head]) {
            *(buffer++) = head << 32 | range;
        }
    }
}

void export_basic_ranges(uint32_t *buffer) {
    auto basic_ranges_ptr = &*BasicRanges::fetch().begin();
    memcpy(buffer, basic_ranges_ptr, BASIC_RANGES_SIZE * 4); // 32-bits -> 4-bytes
}
