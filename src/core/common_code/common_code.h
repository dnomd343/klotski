#pragma once

/// CommonCode is a generic klotski encoding that records an valid case using
/// 36-bit lengths, and stored in a `uint64_t`. Since there is only one `2x2`
/// block, it is encoded separately. Its upper-left corner is called `head`,
/// which has 12 possible positions and is encoded using 4-bit length.
///
///   00 01 02 03
///   04 05 06 07    00 01 02
///   08 09 10 11    04 05 06  <- head of 2x2 block (4-bit)
///   12 13 14 15    08 09 10     (without 03/07/11/15)
///   16 17 18 19    12 13 14

/// Treat spaces as special blocks, there can be four kinds of blocks in total,
/// namely `space`, `1x2`, `2x1`, `1x1`. Each of them is represented by 2-bit,
/// which are `00` `01` `10` `11`. Arrange them according to their position and
/// size, and we can get a binary sequence.
///
///   ( 2x2 -> # # )  |  ( 2x1 -> # )  |  ( 1x2 -> # # )  |  ( 1x1 -> # )
///   (        # # )  |  (        # )  |                  |

/// This sequence can have up to 16 blocks, aka 32-bit in length. Therefore, in
/// order to be compatible with all klotski cases, the length of this part of
/// the code is set to 32-bit. In addition, for the convenience of reading, it
/// is stipulated that the sequence starts from the high bit, and the remaining
/// bits should be filled with `0`.

/// Putting the content of the `head` in the upper 4-bit, and the lower 32-bit
/// to store the sequence content, a 36-bit length code can be obtained, which
/// corresponds to any valid case one-to-one. When CommonCode is converted into
/// a string, just directly export the hexadecimal data, and get a 9-bit string
/// encoding. Characters are not case-sensitive, but it is recommended to use
/// uppercase letters. In addition, the last `0` of the string is allowed to be
/// omitted, and it can be completed to 9 digits when decoding, but note that
/// if the encoding is all `0`, it should be reduced to the remaining one.

/// ----------------------------------------------------------------------------------- ///
///   Eg1:                                                                              ///
///     % # # %    2x2 -> head = 1                                                      ///
///     % # # %    2x1 2x1 2x1 1x2 2x1 1x1 1x1 1x1 space space 1x1 ... ... ... ... ...  ///
///     @ $ $ @     10  10  10  01  10  11  11  11    00    00  11  00  00  00  00  00  ///
///     @ & * @       1010    1001    1011    1111        0000    1100    0000    0000  ///
///     *     &          A       9       B       F           0       C       0       0  ///
///   CommonCode = 0x1A9BF0C00 -> "1A9BF0C"                                             ///
/// ----------------------------------------------------------------------------------- ///

/// ----------------------------------------------------------------------------------- ///
///   Eg2:                                                                              ///
///     * @ & %    2x2 -> head = 4                                                      ///
///     # # $ %    1x1 1x1 1x1 2x1 2x1 2x1 space 1x2 space 1x1 1x2 ... ... ... ... ...  ///
///     # # $ ^     11  11  11  10  10  10    00  01    00  11  01  00  00  00  00  00  ///
///       ~ ~ ^       1111    1110    1010      0001      0011    0100    0000    0000  ///
///       @ % %          F       E       A         1         3       4       0       0  ///
///   CommonCode = 0x4FEA13400 -> "4FEA134"                                             ///
/// ----------------------------------------------------------------------------------- ///

#include <string>
#include <cstdint>
#include <ostream>
#include <optional>

namespace klotski {
namespace codec {

class RawCode;
class ShortCode;
class CommonCode {
public:
    explicit operator uint64_t() const noexcept;
    static bool check(uint64_t common_code) noexcept;
    friend std::ostream& operator<<(std::ostream &out, CommonCode self);

    [[nodiscard]] uint64_t unwrap() const noexcept;
    [[nodiscard]] RawCode to_raw_code() const noexcept;
    [[nodiscard]] ShortCode to_short_code() const noexcept;
    [[nodiscard]] std::string to_string(bool shorten = false) const noexcept;

public:
    CommonCode() = delete;
    explicit CommonCode(RawCode raw_code) noexcept;
    explicit CommonCode(ShortCode short_code) noexcept;

    static CommonCode unsafe_create(uint64_t common_code) noexcept;
    static std::optional<CommonCode> create(uint64_t common_code) noexcept;

    static std::optional<CommonCode> from_string(std::string &&common_code) noexcept;
    static std::optional<CommonCode> from_string(const std::string &common_code) noexcept;

    static CommonCode from_raw_code(RawCode raw_code) noexcept;
    static std::optional<CommonCode> from_raw_code(uint64_t raw_code) noexcept;

    static CommonCode from_short_code(ShortCode short_code) noexcept;
    static std::optional<CommonCode> from_short_code(uint32_t short_code) noexcept;
    static std::optional<CommonCode> from_short_code(std::string &&short_code) noexcept;
    static std::optional<CommonCode> from_short_code(const std::string &short_code) noexcept;

private:
    uint64_t code_;
    static std::string string_encode(uint64_t common_code) noexcept;
    static std::string string_encode_shorten(uint64_t common_code) noexcept;
    static std::optional<uint64_t> string_decode(const std::string &common_code) noexcept;
};

} // namespace codec
} // namespace klotski

#include "inline_impl.h"
