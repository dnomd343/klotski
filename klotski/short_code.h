#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>

const char SHORT_CODE_TABLE[32] = {
    '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'M',
    'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
};

// 1 -> 48
// ...
// 9 -> 57
// A -> 65
// ...
// H -> 72
// J -> 74
// ...
// K -> 75
// M -> 77
// N -> 78
// P -> 80
// ...
// Z -> 90

class ShortCode {
public:
    enum Mode {NORMAL, FAST};

    ShortCode() = default;
    void speed_up(enum Mode mode);
    explicit ShortCode(enum Mode mode);
    static bool check(uint32_t short_code);

    uint32_t zip_short_code(uint64_t common_code);
    uint64_t unzip_short_code(uint32_t short_code);

    static std::string code_to_string(uint32_t short_code);
//    static uint32_t code_from_string(const std::string &short_code);

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
