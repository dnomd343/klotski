#pragma once

#include <mutex>
#include <vector>
#include <cstdint>

class AllCases {
public:
    static std::vector<uint32_t> basic_ranges;

    static void build_basic_ranges();

    static bool basic_ranges_available;

    static std::mutex basic_ranges_building;

};
