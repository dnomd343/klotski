#pragma once

#include <mutex>
#include <vector>
#include <cstdint>
#include <unordered_map>

class ShortCode {
public:

    enum Mode {
        NORMAL, FAST
    };

    static void build_mappings();

    static void speed_up(enum Mode mode);

    static std::mutex map_building;

    static bool fast_mode_available;
    static bool normal_mode_available;


//    static std::vector<uint32_t> *basic_ranges;

    static std::vector<uint64_t> all_cases_list; // short_code -> common_code
    static std::unordered_map<uint64_t, uint32_t> all_cases_dict; // common_code -> short_code

};
