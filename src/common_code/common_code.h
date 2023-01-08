#pragma once

#include <string>
#include <cstdint>
#include "short_code.h"

class ShortCode;

class CommonCode {
public:
    uint64_t unwrap() const;
    ShortCode to_short_code() const;
    static bool check(uint64_t common_code);
    std::string to_string(bool shorten = false) const;

    explicit CommonCode(uint64_t common_code);
    explicit CommonCode(const std::string &common_code_str);

private:
    uint64_t code;
};
