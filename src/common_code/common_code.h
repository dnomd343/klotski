#pragma once

#include <string>
#include <cstdint>
#include "raw_code.h"
#include "short_code.h"

class RawCode;
class ShortCode;

class CommonCode {
public:
    uint64_t unwrap() const;
    static bool check(uint64_t common_code);
    static CommonCode unsafe_create(uint64_t code);

    RawCode to_raw_code() const;
    ShortCode to_short_code() const;
    std::string to_string(bool shorten = false) const;

    explicit CommonCode(uint64_t common_code);
    explicit CommonCode(const RawCode &raw_code);
    explicit CommonCode(const ShortCode &short_code);
    explicit CommonCode(const std::string &common_code_str);

    // TODO: std::cout << CommonCode(...)

    // TODO: CommonCode::create(...) / CommonCode::from_string(...)
    // TODO: CommonCode::from_short_code(...) / CommonCode::from_raw_code(...)

    // TODO: single check function for CommonCode
    // TODO: single `.cc` file for serialize

private:
    uint64_t code;
    CommonCode() = default; // unsafe initialize
};
