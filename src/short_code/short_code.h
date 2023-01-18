#pragma once

#include <string>
#include <cstdint>
#include <ostream>
#include "common_code.h"

const uint32_t SHORT_CODE_LIMIT = 29334498;

class CommonCode;

class ShortCode {
public:
    enum Mode {NORMAL, FAST};

    static void speed_up(enum Mode mode);
    static bool check(uint32_t short_code);
    friend std::ostream& operator<<(std::ostream &out, const ShortCode &self);

    /// Export functions
    uint32_t unwrap() const;
    std::string to_string() const;
    CommonCode to_common_code() const;

    /// ShortCode constructors
    explicit ShortCode(uint32_t short_code);
    explicit ShortCode(const std::string &short_code);
    explicit ShortCode(const CommonCode &common_code);

    ShortCode(uint32_t short_code, enum Mode mode) : ShortCode(short_code) { speed_up(mode); }
    ShortCode(const std::string &short_code, enum Mode mode) : ShortCode(short_code) { speed_up(mode); }
    ShortCode(const CommonCode &common_code, enum Mode mode) : ShortCode(common_code) { speed_up(mode); }

    /// Rust-style initialization
    static ShortCode create(uint32_t short_code);
    static ShortCode from_string(const std::string &short_code);

    static ShortCode from_common_code(uint64_t common_code);
    static ShortCode from_common_code(const CommonCode &common_code);
    static ShortCode from_common_code(const std::string &common_code);

private:
    uint32_t code;
    static bool fast_mode_available;
    static bool normal_mode_available;

    static enum Mode mode();
    static uint64_t fast_decode(uint32_t short_code);
    static uint32_t fast_encode(uint64_t common_code);
    static uint64_t tiny_decode(uint32_t short_code);
    static uint32_t tiny_encode(uint64_t common_code);
};
