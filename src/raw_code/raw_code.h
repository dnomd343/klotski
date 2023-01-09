#pragma once

#include <cstdint>
#include "common_code.h"

#define B_space 0x0
#define B_fill  0x7
#define B_1x2   0x1
#define B_2x1   0x2
#define B_1x1   0x3
#define B_2x2   0x4

class RawCode {
public:
    uint64_t unwrap() const;
    std::string dump_case() const;
    CommonCode to_common_code() const;

    RawCode(const CommonCode &common_code);

    explicit RawCode(uint64_t raw_code) : code(raw_code) {}

private:
    uint64_t code;

};
