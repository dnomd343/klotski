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
    uint32_t unwrap() const;
    std::string to_string() const;
    static bool check(uint32_t short_code);

    static enum Mode check_mode();
    static void speed_up(enum Mode mode);

    explicit ShortCode(uint32_t short_code);
    explicit ShortCode(const std::string &short_code_str);
    ShortCode(uint32_t short_code, enum Mode mode) : ShortCode(short_code) {
        speed_up(mode);
    }
    ShortCode(const std::string &short_code_str, enum Mode mode) : ShortCode(short_code_str) {
        speed_up(mode);
    }

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
