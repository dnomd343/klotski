#pragma once

///                ShortCode Convert Table                ///
///   -------------------------------------------------   ///
///   |  00 |  01 |  02 |  03 |  04 |  05 |  06 |  07 |   ///
///   | `1` | `2` | `3` | `4` | `5` | `6` | `7` | `8` |   ///
///   |-----------------------------------------------|   ///
///   |  08 |  09 |  10 |  11 |  12 |  13 |  14 |  15 |   ///
///   | `9` | `A` | `B` | `C` | `D` | `E` | `F` | `G` |   ///
///   |-----------------------------------------------|   ///
///   |  16 |  17 |  18 |  19 |  20 |  21 |  22 |  23 |   ///
///   | `H` | `J` | `K` | `M` | `N` | `P` | `Q` | `R` |   ///
///   |-----------------------------------------------|   ///
///   |  24 |  25 |  26 |  27 |  28 |  29 |  30 |  31 |   ///
///   | `S` | `T` | `U` | `V` | `W` | `X` | `Y` | `Z` |   ///
///   -------------------------------------------------   ///

namespace klotski::codec {

constexpr char SHORT_CODE_TABLE[32] {
    '1', '2', '3', '4', '5', '6', '7', '8', '9', // skip `0`
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', // skip `I`
    'J', 'K', // skip `L`
    'M', 'N', // skip `O`
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
};

constexpr char SHORT_CODE_TABLE_REV[74] {
     0,  1,  2,  3,  4,  5,  6,  7,  8,     // `1` ~ `9` | [49, 57]
    -1, -1, -1, -1, -1, -1, -1,             //           | [58, 64]
     9, 10, 11, 12, 13, 14, 15, 16, -1, 17, // `A` ~ `J` | [65, 74]
    18, -1, 19, 20, -1, 21, 22, 23, 24, 25, // `K` ~ `T` | [75, 84]
    26, 27, 28, 29, 30, 31,                 // `U` ~ `Z` | [85, 90]
    -1, -1, -1, -1, -1, -1,                 //           | [91, 96]
     9, 10, 11, 12, 13, 14, 15, 16, -1, 17, // `a` ~ `j` | [97, 106]
    18, -1, 19, 20, -1, 21, 22, 23, 24, 25, // `k` ~ `t` | [107, 116]
    26, 27, 28, 29, 30, 31,                 // `u` ~ `z` | [117, 122]
};

} // namespace klotski::codec
