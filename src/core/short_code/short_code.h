#pragma once

/// ShortCode is a high-compression encoding scheme based on CommonCode. Since
/// there are a total of 29334498 valid klotski layouts, arrange their
/// CommonCodes from small to large (36-bit positive integers), and use the
/// index as the ShortCode.

/// Therefore, the valid value of ShortCode is [0, 29334498), stored in
/// `uint32_t`. The goal of high compression ratio is to facilitate verbal
/// sharing, so it is necessary to represent it in a suitable string. Similar
/// to Bitcoin's `base58`, in ShortCode, 4 confusing characters `0` `O` `I` `l`
/// are removed from 10 numbers and 26 characters, forming a private base32
/// scheme.

/// Coincidentally, log(32, 29334498) is approximately equal to `4.96`, so
/// using 5-bit base32 can make good use of space, so any valid klotski layout
/// can be represented by a 5-bit length code. As in CommonCode, the characters
/// here are case insensitive, but uppercase is still recommended.

///                ShortCode Convert Table
///   -------------------------------------------------
///   |  00 |  01 |  02 |  03 |  04 |  05 |  06 |  07 |
///   | `1` | `2` | `3` | `4` | `5` | `6` | `7` | `8` |
///   |-----------------------------------------------|
///   |  08 |  09 |  10 |  11 |  12 |  13 |  14 |  15 |
///   | `9` | `A` | `B` | `C` | `D` | `E` | `F` | `G` |
///   |-----------------------------------------------|
///   |  16 |  17 |  18 |  19 |  20 |  21 |  22 |  23 |
///   | `H` | `J` | `K` | `M` | `N` | `P` | `Q` | `R` |
///   |-----------------------------------------------|
///   |  24 |  25 |  26 |  27 |  28 |  29 |  30 |  31 |
///   | `S` | `T` | `U` | `V` | `W` | `X` | `Y` | `Z` |
///   -------------------------------------------------

///   Eg1:
///     0x1A9BF0C00 -> index 4091296
///     4091296 = 3 * (32 ^ 4) + 28 * (32 ^ 3) + 27 * (32 ^ 2) + 13 * (32 ^ 1) + 0 * (32 ^ 0)
///       => (3), (28), (27), (13), (0)
///       => `4`, `W`, `V`, `E`, `1`
///       => "4WVE1"

///   Eg2:
///     0x4FEA13400 -> index 10399732
///     10399732 = 9 * (32 ^ 4) + 29 * (32 ^ 3) + 11 * (32 ^ 2) + 31 * (32 ^ 1) + 20 * (32 ^ 0)
///       => (9), (29), (11), (31), (20)
///       => `A`, `X`, `C`, `Z`, `N`
///       => "AXCZN"

/// Compared with CommonCode, although ShortCode saves space, it completely
/// loses readability. The former can directly get the layout without the help
/// of a computer, while the latter is almost impossible to complete by the
/// human brain.

#include <string>
#include <cstdint>
#include <ostream>
#include <optional>

namespace klotski {
namespace codec {

const uint32_t SHORT_CODE_LIMIT = 29334498;

class CommonCode;
class ShortCode {
public:
    static void speed_up(bool fast_mode);
    explicit operator uint32_t() const noexcept;
    static bool check(uint32_t short_code) noexcept;
    friend std::ostream& operator<<(std::ostream &out, ShortCode self);

    [[nodiscard]] uint32_t unwrap() const noexcept;
    [[nodiscard]] std::string to_string() const noexcept;
    [[nodiscard]] CommonCode to_common_code() const noexcept;

public:
    ShortCode() = delete;
    explicit ShortCode(CommonCode common_code) noexcept;

    static ShortCode unsafe_create(uint32_t short_code) noexcept;
    static std::optional<ShortCode> create(uint32_t short_code) noexcept;

    static std::optional<ShortCode> from_string(std::string &&short_code) noexcept;
    static std::optional<ShortCode> from_string(const std::string &short_code) noexcept;

    static ShortCode from_common_code(CommonCode common_code) noexcept;
    static std::optional<ShortCode> from_common_code(uint64_t common_code) noexcept;
    static std::optional<ShortCode> from_common_code(std::string &&common_code) noexcept;
    static std::optional<ShortCode> from_common_code(const std::string &common_code) noexcept;

private:
    uint32_t code_;
    static bool fast_available_;

    static uint64_t fast_decode(uint32_t short_code) noexcept;
    static uint32_t fast_encode(uint64_t common_code) noexcept;
    static uint64_t tiny_decode(uint32_t short_code) noexcept;
    static uint32_t tiny_encode(uint64_t common_code) noexcept;

    static std::string string_encode(uint32_t short_code) noexcept;
    static std::optional<uint32_t> string_decode(const std::string &short_code) noexcept;
};

/// CommonCode compare implements.
inline bool operator==(uint32_t s1, ShortCode s2) noexcept {
    return s1 == s2.unwrap();
}

inline bool operator==(ShortCode s1, ShortCode s2) noexcept {
    return s1.unwrap() == s2.unwrap();
}

inline bool operator<(ShortCode s1, ShortCode s2) noexcept {
    return s1.unwrap() < s2.unwrap();
}

inline bool operator>(ShortCode s1, ShortCode s2) noexcept {
    return s1.unwrap() > s2.unwrap();
}

/// Get the original 32-bit code.
inline uint32_t ShortCode::unwrap() const noexcept {
    return code_;
}

/// Implicit conversion to 32-bit code.
inline ShortCode::operator uint32_t() const noexcept {
    return code_;
}

/// ShortCode create without any check.
inline ShortCode ShortCode::unsafe_create(uint32_t short_code) noexcept {
    return *reinterpret_cast<ShortCode*>(&short_code); // init directly
}

/// CommonCode create with valid check.
inline std::optional<ShortCode> ShortCode::create(uint32_t short_code) noexcept {
    if (ShortCode::check(short_code)) {
        return ShortCode::unsafe_create(short_code);
    }
    return std::nullopt;
}

/// Output string encoding of ShortCode.
inline std::ostream& operator<<(std::ostream &out, ShortCode self) {
    out << ShortCode::string_encode(self.code_);
    return out;
}

} // namespace codec
} // namespace klotski
