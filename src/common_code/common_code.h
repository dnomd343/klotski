#pragma once

#include <string>
#include <cstdint>
#include "short_code.h"

class ShortCode;

class CommonCode {
public:
    uint64_t unwrap() const;
    static bool check(uint64_t common_code);
    static CommonCode unsafe_create(uint64_t code);

    // TODO: to raw code function
    ShortCode to_short_code() const;
    std::string to_string(bool shorten = false) const;

    explicit CommonCode(uint64_t common_code);
    // TODO: init with const RawCode&
    explicit CommonCode(const ShortCode &short_code);
    explicit CommonCode(const std::string &common_code_str);

private:
    uint64_t code;
    CommonCode() = default; // unsafe init
};
