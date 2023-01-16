#pragma once

#include <mutex>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include "common_code.h"

class CommonCode;

class ShortCode {
public:
    enum Mode {
        NORMAL, FAST
    };
    static enum Mode check_mode();
    static void speed_up(enum Mode mode);

    uint32_t unwrap() const;
    std::string to_string() const;
    CommonCode to_common_code() const;
    static bool check(uint32_t short_code);

    // TODO: std::cout << ShortCode(...) << std::endl;

    explicit ShortCode(uint32_t short_code);
    explicit ShortCode(const CommonCode &common_code);
    explicit ShortCode(const std::string &short_code_str);
    ShortCode(uint32_t short_code, enum Mode mode) : ShortCode(short_code) {
        speed_up(mode);
    }
    ShortCode(const CommonCode &common_code, enum Mode mode) : ShortCode(common_code) {
        speed_up(mode);
    }
    ShortCode(const std::string &short_code_str, enum Mode mode) : ShortCode(short_code_str) {
        speed_up(mode);
    }

    // TODO: mark as private after inner test
    static uint64_t fast_decode(uint32_t short_code);
    static uint32_t fast_encode(uint64_t common_code);

    static uint64_t tiny_decode(uint32_t short_code);
    static uint32_t tiny_encode(uint64_t common_code);

    // TODO: ShortCode::create() / ShortCode::from_str(...) / ShortCode::from_common_code(...)

    static uint32_t fast_encode_legacy(uint64_t common_code);

private:
    uint32_t code;

    static std::mutex map_building;
    static bool fast_mode_available;
    static bool normal_mode_available;

    static const uint32_t SHORT_CODE_LIMIT = 29334498;

    /// NOTE: using binary search instead of global vector and unordered_map
    ///       for some test, the new function only using < 170MB memory, while the legacy using > 1.5GB
    ///       but the legacy one is more easy and a little fast.
    ///       BTW, the new one init less than 1.2s, legacy one need about 15s
    static std::vector<uint64_t> all_cases_list; // short_code -> common_code
    static std::unordered_map<uint64_t, uint32_t> all_cases_dict; // common_code -> short_code

    static void build_mappings();
//    static uint64_t tiny_decode(uint32_t short_code);
//    static uint32_t tiny_encode(uint64_t common_code);
};
