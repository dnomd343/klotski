/// Klotski Engine by Dnomd343 @2024

/// CommonCode is a generic klotski encoding that records a valid case using
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
/// which are `00` `01` `10` `11`. Arrange them in ascending order according to
/// the position numbers corresponding to their upper left corners, and we will
/// get a binary sequence.
///   ┌────────────┬──────────┬────────────┬──────────┐
///   │ (2x2) ████ │ (2x1) ██ │ (1x2) ████ │ (1x1) ██ │
///   │       ████ │       ██ │            │          │
///   └────────────┴──────────┴────────────┴──────────┘

/// This sequence can have up to 16 blocks, aka 32-bit. Therefore, in order to
/// be compatible with all klotski cases, the length of this part is set to
/// 32-bit. In addition, for the convenience of reading, it is stipulated that
/// the sequence starts from the high bit, and the remaining bits should be
/// filled with `0`.

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
///     ┏━┳━━━┳━┓                                                                       ///
///     ┃ ┃   ┃ ┃  2x2 -> head = 1                                                      ///
///     ┣━╋━━━╋━┫  2x1 2x1 2x1 1x2 2x1 1x1 1x1 1x1 space space 1x1 ... ... ... ... ...  ///
///     ┃ ┣━┳━┫ ┃   10  10  10  01  10  11  11  11    00    00  11  00  00  00  00  00  ///
///     ┣━╋━┻━╋━┫     1010    1001    1011    1111        0000    1100    0000    0000  ///
///     ┗━┛   ┗━┛        A       9       B       F           0       C       0       0  ///
///   CommonCode = 0x1A9BF0C00 -> "1A9BF0C"                                             ///
/// ----------------------------------------------------------------------------------- ///

/// ----------------------------------------------------------------------------------- ///
///   Eg2:                                                                              ///
///     ┏━┳━┳━┳━┓                                                                       ///
///     ┣━┻━╋━┫ ┃  2x2 -> head = 4                                                      ///
///     ┃   ┃ ┣━┫  1x1 1x1 1x1 2x1 2x1 2x1 space 1x2 space 1x1 1x2 ... ... ... ... ...  ///
///     ┗━┳━┻━┫ ┃   11  11  11  10  10  10    00  01    00  11  01  00  00  00  00  00  ///
///       ┣━┳━┻━┫     1111    1110    1010      0001      0011    0100    0000    0000  ///
///       ┗━┻━━━┛        F       E       A         1         3       4       0       0  ///
///   CommonCode = 0x4FEA13400 -> "4FEA134"                                             ///
/// ----------------------------------------------------------------------------------- ///

#pragma once

#include <string>
#include <cstdint>
#include <optional>

#include "utils/utility.h"
#include "raw_code/raw_code_fwd.h"
#include "short_code/short_code_fwd.h"

namespace klotski::codec {

class CommonCode {
public:
    // ------------------------------------------------------------------------------------- //

    CommonCode() = delete;

    /// Construct CommonCode from RawCode.
    explicit constexpr CommonCode(RawCode raw_code);

    /// Construct CommonCode from ShortCode.
    explicit CommonCode(ShortCode short_code);

    /// Create CommonCode without any check.
    static constexpr CommonCode unsafe_create(uint64_t common_code);

    /// Create CommonCode with validity check.
    static constexpr std::optional<CommonCode> create(uint64_t common_code);

    // ------------------------------------------------------------------------------------- //

    /// Explicit conversion to u64 code.
    explicit constexpr operator uint64_t() const;

    /// Check the validity of the original CommonCode.
    static constexpr bool check(uint64_t common_code);

#ifndef KLSK_NDEBUG
    /// Output string encoding of CommonCode only for debug.
    friend std::ostream& operator<<(std::ostream &out, CommonCode self);
#endif

    // ------------------------------------------------------------------------------------- //

    /// Get the original u64 code.
    [[nodiscard]] constexpr uint64_t unwrap() const;

    /// Convert CommonCode to RawCode.
    [[nodiscard]] constexpr RawCode to_raw_code() const;

    /// Convert CommonCode to ShortCode.
    [[nodiscard]] ShortCode to_short_code() const;

    /// Convert CommonCode to string form.
    [[nodiscard]] std::string to_string(bool shorten = false) const;

    // ------------------------------------------------------------------------------------- //

    /// Create CommonCode from string form.
    static std::optional<CommonCode> from_string(std::string_view common_code);

    // ------------------------------------------------------------------------------------- //

    /// Create CommonCode from RawCode.
    static constexpr CommonCode from_raw_code(RawCode raw_code);

    /// Create CommonCode from RawCode in u64.
    static constexpr std::optional<CommonCode> from_raw_code(uint64_t raw_code);

    // ------------------------------------------------------------------------------------- //

    /// Create CommonCode from ShortCode.
    static CommonCode from_short_code(ShortCode short_code);

    /// Create CommonCode from ShortCode in u32.
    static std::optional<CommonCode> from_short_code(uint32_t short_code);

    /// Create CommonCode from ShortCode in string form.
    static std::optional<CommonCode> from_short_code(std::string_view short_code);

    // ------------------------------------------------------------------------------------- //

    /// Whether the layout is vertically symmetrical.
    [[nodiscard]] constexpr bool is_vertical_mirror() const;

    /// Whether the layout is horizontally symmetrical.
    [[nodiscard]] constexpr bool is_horizontal_mirror() const;

    /// Calculate the vertically symmetrical klotski layout.
    [[nodiscard]] constexpr CommonCode to_vertical_mirror() const;

    /// Calculate the horizontally symmetrical klotski layout.
    [[nodiscard]] constexpr CommonCode to_horizontal_mirror() const;

    // ------------------------------------------------------------------------------------- //

    /// Compare CommonCode with u64 value.
    friend constexpr auto operator==(const CommonCode &lhs, uint64_t rhs);
    friend constexpr auto operator<=>(const CommonCode &lhs, uint64_t rhs);

    /// Compare the original values of two CommonCodes.
    friend constexpr auto operator==(const CommonCode &lhs, const CommonCode &rhs);
    friend constexpr auto operator<=>(const CommonCode &lhs, const CommonCode &rhs);

    // ------------------------------------------------------------------------------------- //

private:
    uint64_t code_;

    // ------------------------------------------------------------------------------------- //

    /// Serialize CommonCode into 9-bit length string.
    static std::string string_encode(uint64_t common_code);

    /// Serialize CommonCode into a variable-length string without trailing zero.
    static std::string string_encode_shorten(uint64_t common_code);

    /// Deserialize CommonCode from string and return nullopt on error.
    static std::optional<uint64_t> string_decode(std::string_view common_code);

    // ------------------------------------------------------------------------------------- //

    /// Check the horizontally symmetrical.
    static constexpr bool check_mirror(uint64_t common_code);

    /// Get the vertically symmetrical layout.
    static constexpr uint64_t get_vertical_mirror(uint64_t common_code);

    /// Get the horizontally symmetrical layout.
    static constexpr uint64_t get_horizontal_mirror(uint64_t common_code);

    // ------------------------------------------------------------------------------------- //
};

static_assert(sizeof(CommonCode) == 8);
static_assert(is_compact_layout_v<CommonCode>);
static_assert(std::is_standard_layout_v<CommonCode>);
static_assert(std::is_trivially_copyable_v<CommonCode>);

} // namespace klotski::codec

#include "internal/common_code.inl"
#include "internal/serialize.inl"
#include "internal/mirror.inl"
#include "internal/check.inl"

template <>
struct std::hash<klotski::codec::CommonCode> {
    constexpr std::size_t operator()(const klotski::codec::CommonCode &c) const noexcept {
        return std::hash<uint64_t>{}(c.unwrap());
    }
};
