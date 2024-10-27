#pragma once

/// Klotski is an intellectual game, which fill a `5x4` chessboard with movable
/// pieces (called blocks), and a valid layout obeys the following three
/// requirements:
///
///   1. There are four types of blocks, namely `2x2` `2x1` `1x2` `1x1`.
///
///   2. There should be at least two free slots (called space).
///
///   3. `2x2` block must have and only one, `2x1` `1x2` `1x1` are not required
///      in number.
///
///  NOTE:  2x2 -> # #   |   2x1 -> #   |   1x2 -> # #   |   1x1 -> #
///                # #   |          #   |                |
///
/// After statistics, there are a total of `29334498` cases that meet the above
/// requirements.

#include <cstdint>

// TODO: using constexpr

namespace klotski {

// TODO: remove `CE` tag
constexpr uint_fast8_t BLOCK_SPC    {0b000}; // space
constexpr uint_fast8_t BLOCK_FIL    {0b111}; // fill
constexpr uint_fast8_t BLOCK_CE_1x2 {0b001};
constexpr uint_fast8_t BLOCK_CE_2x1 {0b010};
constexpr uint_fast8_t BLOCK_CE_1x1 {0b011};
constexpr uint_fast8_t BLOCK_CE_2x2 {0b100};
constexpr uint_fast8_t BLOCK_MSK    {0b111}; // mask

} // namespace klotski

/// NOTE: 0b101 and 0b110 are reserved
#define BLOCK_space 0b000
#define BLOCK_fill  0b111
#define BLOCK_1x2   0b001
#define BLOCK_2x1   0b010
#define BLOCK_1x1   0b011
#define BLOCK_2x2   0b100

/// The four types of blocks are recorded using the following format.

///    1x1    |        1x2         |    2x1     |        2x2
///  [B_1x1]  |  [B_1x2] [B_fill]  |  [B_2x1]   |  [B_2x2]  [B_fill]
///           |                    |  [B_fill]  |  [B_fill] [B_fill]

/// Mapped to the chessboard, it can be expressed with the following 60-bit data.

///       C_1x1       |       C_1x2       |       C_2x1       |       C_2x2
///  011 000 000 000  |  001 111 000 000  |  010 000 000 000  |  100 111 000 000
///  000 000 000 000  |  000 000 000 000  |  111 000 000 000  |  111 111 000 000
///  000 000 000 000  |  000 000 000 000  |  000 000 000 000  |  000 000 000 000
///  000 000 000 000  |  000 000 000 000  |  000 000 000 000  |  000 000 000 000
///  000 000 000 000  |  000 000 000 000  |  000 000 000 000  |  000 000 000 000

#define K_MASK_1x1 (uint64_t)0x00003 // 000 000 000 000 000 011
#define K_MASK_1x2 (uint64_t)0x00039 // 000 000 000 000 111 001
#define K_MASK_2x1 (uint64_t)0x07002 // 000 111 000 000 000 010
#define K_MASK_2x2 (uint64_t)0x3F03C // 111 111 000 000 111 100

/// Express block coverage with the following data.

///       F_1x1       |       F_1x2       |       F_2x1       |       F_2x2
///  111 000 000 000  |  111 111 000 000  |  111 000 000 000  |  111 111 000 000
///  000 000 000 000  |  000 000 000 000  |  111 000 000 000  |  111 111 000 000
///  000 000 000 000  |  000 000 000 000  |  000 000 000 000  |  000 000 000 000
///  000 000 000 000  |  000 000 000 000  |  000 000 000 000  |  000 000 000 000
///  000 000 000 000  |  000 000 000 000  |  000 000 000 000  |  000 000 000 000

#define K_MASK_1x1_ (uint64_t)0x00007 // 000 000 000 000 000 111
#define K_MASK_1x2_ (uint64_t)0x0003F // 000 000 000 000 111 111
#define K_MASK_2x1_ (uint64_t)0x07007 // 000 111 000 000 000 111
#define K_MASK_2x2_ (uint64_t)0x3F03F // 111 111 000 000 111 111

///      F_1x1_R      |      F_1x1_D      |      F_2x1_R      |      F_1x2_D
///  000 111 000 000  |  000 000 000 000  |  000 111 000 000  |  000 000 000 000
///  000 000 000 000  |  111 000 000 000  |  000 111 000 000  |  111 111 000 000
///  000 000 000 000  |  000 000 000 000  |  000 000 000 000  |  000 000 000 000
///  000 000 000 000  |  000 000 000 000  |  000 000 000 000  |  000 000 000 000
///  000 000 000 000  |  000 000 000 000  |  000 000 000 000  |  000 000 000 000

#define K_MASK_1x1_R (uint64_t)0x00038 // 000 000 000 000 111 000
#define K_MASK_1x1_D (uint64_t)0x07000 // 000 111 000 000 000 000
#define K_MASK_2x1_R (uint64_t)0x38038 // 111 000 000 000 111 000
#define K_MASK_1x2_D (uint64_t)0x3F000 // 111 111 000 000 000 000
