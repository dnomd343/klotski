#pragma once

#include <mutex>
#include <vector>
#include <cstdint>
#include "basic_ranges.h"

class AllCases : public BasicRanges {
public:
    static void build();
    static enum Status status();
    static const std::vector<uint32_t> (*fetch())[16];

private:
    static bool all_cases_available;
    static std::mutex all_cases_building;
    static std::vector<uint32_t> all_cases[16];
};