#pragma once

#include <vector>
#include <cstdint>
#include <unordered_map>

class ShortCode {
public:

    std::vector<uint64_t> all_cases_list;                  // short_code -> common_code
    std::unordered_map<uint64_t, uint32_t> all_cases_dict; // common_code -> short_code

    void speed_up();

//    uint32_t zip_short_code(uint64_t code);
//    uint64_t unzip_short_code(uint32_t short_code);

    uint64_t fast_decode(uint32_t short_code);
    uint32_t fast_encode(uint64_t common_code);

private:
//    std::vector<uint64_t> all_cases_list;                  // short_code -> common_code
//    std::unordered_map<uint64_t, uint32_t> all_cases_dict; // common_code -> short_code

    void build_mapping();


};
