#pragma once

/// CommonCode is a generic klotski encoding that records an valid case using
/// 36-bits lengths, and stored in a `uint64_t`.

/// Since there is only one `2x2` block, it is encoded separately. Its upper
/// left corner is called `head`, it has 12 possible positions and is encoded
/// using 4-bits length (0 ~ 15).
///
///   00 01 02 03
///   04 05 06 07    00 01 02
///   08 09 10 11    04 05 06  <- head of 2x2 block
///   12 13 14 15    08 09 10     (without 03/07/11/15)
///   16 17 18 19    12 13 14

/// Treat spaces as special blocks, there can be four kinds of blocks in total,
/// namely `space`, `1x2`, `2x1`, `1x1`. Each of them is represented by 2-bits,
/// which are `00` `01` `10` `11`. Arrange them according to their position and
/// size, and we can get a binary sequence.
///   2x2 -> # #   |   2x1 -> #   |   1x2 -> # #   |   1x1 -> #
///          # #   |          #   |                |

/// This sequence can have up to 16 blocks, aka 32-bits in length. Therefore, in
/// order to be compatible with all cases, the length of this part of the code
/// is set to 32-bits. In addition, for the convenience of reading, it is
/// stipulated that the sequence starts from the high bit, and the remaining bits
/// should be filled with `0`.

/// Putting the content of the `head` in the upper 4-bits, and the lower 32-bits
/// to store the sequence content, a 36-bits length code can be obtained, which
/// corresponds to any valid layout one-to-one. When CommonCode is converted into
/// a string, just directly export the hexadecimal data, and get a 9-bits string
/// encoding. Characters are not case-sensitive, but it is recommended to use
/// uppercase letters. In addition, the last `0` of the string is allowed to be
/// omitted, and it can be completed to 9 digits when decoding, but note that if
/// the encoding is all `0`, it should be reduced to the remaining one `0`.

///   Eg1:
///     % # # %    2x2 -> head = 1
///     % # # %    2x1 2x1 2x1 1x2 2x1 1x1 1x1 1x1 space space 1x1 ... ... ... ... ...
///     @ $ $ @     10  10  10  01  10  11  11  11    00    00  11  00  00  00  00  00
///     @ & * @       1010    1001    1011    1111        0000    1100    0000    0000
///     *     &          A       9       B       F           0       C       0       0
///                CommonCode = 0x1A9BF0C00 -> "1A9BF0C"

///   Eg2:
///     * @ & %    2x2 -> head = 4
///     # # $ %    1x1 1x1 1x1 2x1 2x1 2x1 space 1x2 space 1x1 1x2 ... ... ... ... ...
///     # # $ ^     11  11  11  10  10  10    00  01    00  11  01  00  00  00  00  00
///       ~ ~ ^       1111    1110    1010      0001      0011    0100    0000    0000
///       @ % %          F       E       A         1         3       4       0       0
///                CommonCode = 0x4FEA13400 -> "4FEA134"

#include <string>
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include "raw_code.h"
#include "short_code.h"

namespace klotski {
    class RawCode; // import for convert interface
    class ShortCode;

    class CommonCodeException : public std::runtime_error {
    public:
        CommonCodeException() : std::runtime_error("invalid common code") {}
        explicit CommonCodeException(const std::string &msg) : std::runtime_error(msg) {}
        ~CommonCodeException() noexcept override = default;
    };

    class CommonCode {
        uint64_t code;
        CommonCode() = default; // unsafe initialize

        static inline uint64_t string_decode(const std::string &common_code);
        static inline std::string string_encode(uint64_t common_code, bool shorten);

    public:
        /// CommonCode validity check
        bool valid() const;
        static bool check(uint64_t common_code);

        /// Operators of CommonCode
        bool operator==(const CommonCode &common_code) const;
        bool operator!=(const CommonCode &common_code) const;
        constexpr explicit operator uint64_t() const { return code; }
        friend std::ostream& operator<<(std::ostream &out, const CommonCode &self);

        /// Export functions
        RawCode to_raw_code() const;
        ShortCode to_short_code() const;
        std::string to_string(bool shorten = false) const;
        constexpr uint64_t unwrap() const { return code; }

        /// CommonCode constructors
        explicit CommonCode(uint64_t common_code);
        explicit CommonCode(RawCode &&raw_code);
        explicit CommonCode(ShortCode &&short_code);
        explicit CommonCode(std::string &&common_code);
        explicit CommonCode(const RawCode &raw_code);
        explicit CommonCode(const ShortCode &short_code);
        explicit CommonCode(const std::string &common_code);

        /// Static initialization
        static CommonCode create(uint64_t common_code);
        static CommonCode unsafe_create(uint64_t common_code);

        static CommonCode from_string(std::string &&common_code);
        static CommonCode from_string(const std::string &common_code);

        static CommonCode from_raw_code(uint64_t raw_code);
        static CommonCode from_raw_code(RawCode &&raw_code);
        static CommonCode from_raw_code(const RawCode &raw_code);

        static CommonCode from_short_code(uint32_t short_code);
        static CommonCode from_short_code(ShortCode &&short_code);
        static CommonCode from_short_code(std::string &&short_code);
        static CommonCode from_short_code(const ShortCode &short_code);
        static CommonCode from_short_code(const std::string &short_code);
    };

    inline bool operator==(uint64_t c1, const CommonCode &c2) { return c1 == c2.unwrap(); }
    inline bool operator!=(uint64_t c1, const CommonCode &c2) { return c1 != c2.unwrap(); }
    inline bool operator==(const CommonCode &c1, uint64_t c2) { return c1.unwrap() == c2; }
    inline bool operator!=(const CommonCode &c1, uint64_t c2) { return c1.unwrap() != c2; }
}
