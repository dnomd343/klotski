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
    static enum Mode check_mode();
    static void speed_up(enum Mode mode);

    static bool check(uint32_t short_code);

    uint32_t unwrap() const;

    std::string to_string() const;

    explicit ShortCode(uint32_t short_code);
//    explicit CommonCode(const std::string &common_code_str);



private:
    uint32_t code;

    static std::mutex map_building;
    static bool fast_mode_available;
    static bool normal_mode_available;
    static const uint32_t SHORT_CODE_LIMIT = 29334498;
    static std::vector<uint64_t> all_cases_list; // short_code -> common_code
    static std::unordered_map<uint64_t, uint32_t> all_cases_dict; // common_code -> short_code

    static void build_mappings();
};
