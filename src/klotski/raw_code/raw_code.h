#pragma once

/// RawCode is an uncompressed coding scheme, which is used for program calculation.
/// It encodes a `5x4` chessboard as `0 ~ 19`, and uses 3-bits to represent each position,
/// occupying a total of 60-bits, and stored in a `uint64_t` variable.
///
///   00 01 02 03
///   04 05 06 07    fill  20-slots
///   08 09 10 11    0000  (19) (18) (17) (16) ... (03) (02) (01) (00)
///   12 13 14 15    (4b)  (60b)
///   16 17 18 19
///
///   Eg1:
///     % # # %    2x1 2x2 ... 2x1    010 100 111 010
///     % # # %    ... ... ... ...    111 111 111 111
///     @ $ $ @    2x1 1x2 ... 2x1    010 001 111 010
///     @ & * @    ... 1x1 1x1 ...    111 011 011 111
///     *     &    1x1 0x0 0x0 1x1    011 000 000 011
///
///            19  18  17  16  15  14  13  12  11  10  09  08  07  06  05  04  03  02  01  00
///      0000 011 000 000 011 111 011 011 111 010 111 001 010 111 111 111 111 010 111 100 010
///      0000  0110 0000 0011  1110 1101 1111  0101 1100 1010  1111 1111 1111  0101 1110 0010
///         0     6    0    3     E    D    F     5    C    A     F    F    F     5    E    2
///
///   Eg2:
///     * @ & %    1x1 1x1 1x1 2x1    011 011 011 010
///     # # $ %    2x2 ... 2x1 ...    100 111 010 111
///     # # $ ^    ... ... ... 2x1    111 111 111 010
///       ~ ~ ^    0x0 1x2 ... ...    000 001 111 111
///       @ % %    0x0 1x1 1x2 ...    000 011 001 111
///
///            19  18  17  16  15  14  13  12  11  10  09  08  07  06  05  04  03  02  01  00
///      0000 111 001 011 000 111 111 001 000 010 111 111 111 111 010 111 100 010 011 011 011
///      0000  1110 0101 1000  1111 1100 1000  0101 1111 1111  1110 1011 1100  0100 1101 1011
///         0     E    5    8     F    C    8     5    F    F     E    B    C     4    D    B

#include <string>
#include <cstdint>
#include <ostream>
#include "common_code.h"

namespace klotski {
    /// import for convert interface
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
}
