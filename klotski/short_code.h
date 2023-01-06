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

const char SHORT_CODE_TABLE_REV[42] = {
//  00  01  02  03  04  05  06  07  08
     0,  1,  2,  3,  4,  5,  6,  7,  8,
//  09  10  11  12  13  14  15
    -1, -1, -1, -1, -1, -1, -1,
//  16  17  18  19  20  21  22  23  24  25
     9, 10, 11, 12, 13, 14, 15, 16, -1, 17,
//  26  27  28  29  30  31  32  33  34  35
    18, -1, 19, 20, -1, 21, 22, 23, 24, 25,
//  36  37  38  39  40  41
    26, 27, 28, 29, 30, 31,
};

// 00: 1 -> 49 (00)
// ...
// 08: 9 -> 57 (08)
// 09: A -> 65 (16)
// ...
// 16: H -> 72 (23)
// 17: J -> 74 (25)
// 18: K -> 75 (26)
// 19: M -> 77 (28)
// 20: N -> 78 (29)
// 21: P -> 80 (31)
// ...
// 31: Z -> 90 (41)

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
    static uint32_t code_from_string(const std::string &short_code);

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
