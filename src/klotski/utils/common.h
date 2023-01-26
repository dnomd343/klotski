#pragma once

/// Klotski is an intellectual game, which fill a `5x4` chessboard with movable pieces
/// (or called blocks), and a valid layout obeys the following three requirements:
///
///   1. There are four types of blocks, namely `2x2` `2x1` `1x2` `1x1`.
///
///   2. There should be at least two free slots (or called space).
///
///   3. `2x2` block must have and only one, `2x1` `1x2` `1x1` are not required in number.
///
/// After statistics, there are a total of 29334498 cases that meet the above requirements.

#include <cstdint>

/// 0b101 and 0b110 reserved
#define B_space 0b000
#define B_fill  0b111
#define B_1x2   0b001
#define B_2x1   0b010
#define B_1x1   0b011
#define B_2x2   0b100

///  C_1x1            |  C_1x2            |  C_2x1            |  C_2x2
///  011 000 000 000  |  001 111 000 000  |  010 000 000 000  |  100 111 000 000
///  000 000 000 000  |  000 000 000 000  |  111 000 000 000  |  111 111 000 000
///  ...              |  ...              |  ...              |  ...

#define C_1x1 (uint64_t)0x3 // 011
#define C_1x2 (uint64_t)0x39 // 111 001
#define C_2x1 (uint64_t)0x7002 // 111 000 000 000 010
#define C_2x2 (uint64_t)0x3F03C // 111 111 000 000 111 100

///  F_1x1            |  F_1x2            |  F_2x1            |  F_2x2
///  111 000 000 000  |  111 111 000 000  |  111 000 000 000  |  111 111 000 000
///  000 000 000 000  |  000 000 000 000  |  111 000 000 000  |  111 111 000 000
///  ...              |  ...              |  ...              |  ...

#define F_1x1 (uint64_t)0x7 // 111
#define F_1x2 (uint64_t)0x3F // 111 111
#define F_2x1 (uint64_t)0x7007 // 111 000 000 000 111
#define F_2x2 (uint64_t)0x3F03F // 111 111 000 000 111 111

///  F_1x1_R          |  F_1x1_D          |  F_2x1_R          |  F_1x2_D
///  000 111 000 000  |  000 000 000 000  |  000 111 000 000  |  000 000 000 000
///  000 000 000 000  |  111 000 000 000  |  000 111 000 000  |  111 111 000 000
///  ...              |  ...              |  ...              |  ...

#define F_1x1_R (uint64_t)0x38 // 111 000
#define F_1x1_D (uint64_t)0x7000 // 111 000 000 000 000
#define F_2x1_R (uint64_t)0x38038 // 111 000 000 000 111 000
#define F_1x2_D (uint64_t)0x3F000 // 111 111 000 000 000 000

/// `range_reverse` flip a 32bit data every two bits
///   00 01 10 11 ...  =>  ... 11 10 01 00
///   (high 8-bits)           (low 8-bits)

class Common {
public:
    static uint32_t range_reverse(uint32_t bin);
    static uint8_t check_range(uint32_t head, uint32_t range);
};
