#pragma once

#include <cstdint>

#define B_space 0b000
#define B_fill  0b111
#define B_1x2   0b001
#define B_2x1   0b010
#define B_1x1   0b011
#define B_2x2   0b100

#define C_1x1 (uint64_t)0x3 // 011
#define C_1x2 (uint64_t)0x39 // 111 001
#define C_2x1 (uint64_t)0x7002 // 111 000 000 000 010
#define C_2x2 (uint64_t)0x3F03C // 111 111 000 000 111 100

///  C_1x1            |  C_1x2            |  C_2x1            |  C_2x2
///  011 000 000 000  |  001 111 000 000  |  010 000 000 000  |  100 111 000 000
///  000 000 000 000  |  000 000 000 000  |  111 000 000 000  |  111 111 000 000
///  ...              |  ...              |  ...              |  ...

#define F_1x1 (uint64_t)0x7 // 111
#define F_1x2 (uint64_t)0x3F // 111 111
#define F_2x1 (uint64_t)0x7007 // 111 000 000 000 111
#define F_2x2 (uint64_t)0x3F03F // 111 111 000 000 111 111

///  F_1x1            |  F_1x2            |  F_2x1            |  F_2x2
///  111 000 000 000  |  111 111 000 000  |  111 000 000 000  |  111 111 000 000
///  000 000 000 000  |  000 000 000 000  |  111 000 000 000  |  111 111 000 000
///  ...              |  ...              |  ...              |  ...

#define F_1x1_R (uint64_t)0x38 // 111 000
#define F_1x1_D (uint64_t)0x7000 // 111 000 000 000 000
#define F_2x1_R (uint64_t)0x38038 // 111 000 000 000 111 000
#define F_1x2_D (uint64_t)0x3F000 // 111 111 000 000 000 000

///  F_1x1_R          |  F_1x1_D          |  F_2x1_R          |  F_1x2_D
///  000 111 000 000  |  000 000 000 000  |  000 111 000 000  |  000 000 000 000
///  000 000 000 000  |  111 000 000 000  |  000 111 000 000  |  111 111 000 000
///  ...              |  ...              |  ...              |  ...

class Common {
public:
    static uint32_t range_reverse(uint32_t bin);
    static bool check_case(uint32_t head, uint32_t range);
};
