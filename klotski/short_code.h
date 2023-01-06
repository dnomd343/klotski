#pragma once

#include <vector>
#include <cstdint>
#include <unordered_map>

class ShortCode {
public:
    enum Mode {NORMAL, FAST};

    ShortCode() = default;
    void speed_up(enum Mode mode);
    explicit ShortCode(enum Mode mode);
    static bool check(uint32_t short_code);
    uint32_t zip_short_code(uint64_t common_code);
    uint64_t unzip_short_code(uint32_t short_code);

private:
    static const uint32_t SHORT_CODE_LIMIT = 29334498;

    std::vector<uint32_t> basic_ranges;
    std::vector<uint64_t> all_cases_list; // short_code -> common_code
    std::unordered_map<uint64_t, uint32_t> all_cases_dict; // common_code -> short_code

    void build_mappings();
    enum Mode check_mode();
    void build_base_ranges();
    uint64_t tiny_decode(uint32_t short_code);
    uint32_t tiny_encode(uint64_t common_code);
};
