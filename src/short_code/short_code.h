#pragma once

#include <cstdint>
#include "common_code.h"

class CommonCode;

class ShortCode {
public:
    enum Mode {NORMAL, FAST};
    static void speed_up(enum Mode mode);

    uint32_t unwrap() const;
    std::string to_string() const;
    CommonCode to_common_code() const;
    static bool check(uint32_t short_code);

    // TODO: std::cout << ShortCode(...) << std::endl;

    explicit ShortCode(uint32_t short_code);
    explicit ShortCode(const std::string &short_code);
    explicit ShortCode(const CommonCode &common_code);

    static ShortCode create(uint32_t short_code);
    static ShortCode from_string(const std::string &short_code);
    static ShortCode from_common_code(const CommonCode &common_code);

    ShortCode(uint32_t short_code, enum Mode mode) : ShortCode(short_code) { speed_up(mode); }
    ShortCode(const std::string &short_code, enum Mode mode) : ShortCode(short_code) { speed_up(mode); }
    ShortCode(const CommonCode &common_code, enum Mode mode) : ShortCode(common_code) { speed_up(mode); }

private:
    uint32_t code;
    static bool fast_mode_available;
    static bool normal_mode_available;
    static const uint32_t SHORT_CODE_LIMIT = 29334498;

    static enum Mode mode();

    static uint64_t fast_decode(uint32_t short_code);
    static uint32_t fast_encode(uint64_t common_code);
    static uint64_t tiny_decode(uint32_t short_code);
    static uint32_t tiny_encode(uint64_t common_code);
};
