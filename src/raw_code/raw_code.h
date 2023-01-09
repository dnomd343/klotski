#pragma once

#include <cstdint>
#include "common_code.h"

#define B_space 0b000
#define B_fill  0b111
#define B_1x2   0b001
#define B_2x1   0b010
#define B_1x1   0b011
#define B_2x2   0b100

#define C_1x1 (uint64_t)0x3
#define C_1x2 (uint64_t)0x39
#define C_2x1 (uint64_t)0x7002
#define C_2x2 (uint64_t)0x3F03C

//#define F_1x1 uint64_t(0x7)
//#define F_1x2 uint64_t(0x3F)
//#define F_2x1 uint64_t(0x7007)
//#define F_2x2 uint64_t(0x3F03F)

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
