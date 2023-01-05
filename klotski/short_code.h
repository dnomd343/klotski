#pragma once

#include <vector>
#include <cstdint>
#include <unordered_map>

class ShortCode {
public:

    std::vector<uint64_t> all_cases_list;                  // short_code -> common_code
    std::unordered_map<uint64_t, uint32_t> all_cases_dict; // common_code -> short_code

    void speed_up();


private:
//    std::vector<uint64_t> all_cases_list;                  // short_code -> common_code
//    std::unordered_map<uint64_t, uint32_t> all_cases_dict; // common_code -> short_code

    void build_mapping();


};
