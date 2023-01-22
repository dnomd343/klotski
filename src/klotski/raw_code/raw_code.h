#pragma once

#include <string>
#include <cstdint>
#include <ostream>
#include "common_code.h"

class CommonCode;

class RawCode {
public:
    bool valid() const;
    static bool check(uint64_t raw_code);

    /// Operators of RawCode
    bool operator==(const RawCode &raw_code) const;
    explicit operator uint64_t() const { return code; }
    friend std::ostream& operator<<(std::ostream &out, const RawCode &self);

    /// Export functions
    uint64_t unwrap() const;
    CommonCode to_common_code() const;

    /// RawCode constructors
    explicit RawCode(uint64_t raw_code);
    explicit RawCode(const CommonCode &common_code);

    /// Rust-style initialization
    static RawCode create(uint64_t raw_code);
    static RawCode unsafe_create(uint64_t raw_code);

    static RawCode from_common_code(uint64_t common_code);
    static RawCode from_common_code(const CommonCode &common_code);
    static RawCode from_common_code(const std::string &common_code);

    // TODO: mirror functions

private:
    uint64_t code;
    RawCode() = default; // unsafe initialize

    static uint64_t compact(uint64_t raw_code);
    static uint64_t extract(uint64_t common_code);
};
