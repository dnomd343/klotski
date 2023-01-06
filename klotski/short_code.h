#pragma once

#include <vector>
#include <cstdint>
#include <unordered_map>

class ShortCode {
public:

    enum Mode {NORMAL, FAST};

    std::vector<uint32_t> basic_ranges;

    std::vector<uint64_t> all_cases_list;                  // short_code -> common_code
    std::unordered_map<uint64_t, uint32_t> all_cases_dict; // common_code -> short_code

    ShortCode() = default;
    void speed_up(enum Mode mode);
    explicit ShortCode(enum Mode mode);

    uint32_t zip_short_code(uint64_t common_code);
    uint64_t unzip_short_code(uint32_t short_code);

    enum Mode check_mode();


private:
//    std::vector<uint64_t> all_cases_list;                  // short_code -> common_code
//    std::unordered_map<uint64_t, uint32_t> all_cases_dict; // common_code -> short_code

    const uint32_t SHORT_CODE_LIMIT = 29334498;

    uint64_t fast_decode(uint32_t short_code);
    uint32_t fast_encode(uint64_t common_code);

    uint64_t tiny_decode(uint32_t short_code);
    uint32_t tiny_encode(uint64_t common_code);

    void build_mappings();
    void build_base_ranges();

};
