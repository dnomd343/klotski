#pragma once

/// ShortCode is a high-compression encoding scheme based on CommonCode. Since there
/// are a total of 29334498 valid klotski layouts, arrange their CommonCodes from
/// small to large (36-bit positive integers), and use the index as the ShortCode.

/// Therefore, the valid value of ShortCode is [0, 29334498), stored in `uint32_t`.
/// The goal of high compression ratio is to facilitate verbal sharing, so it is
/// necessary to represent it in a suitable string. Similar to Bitcoin's `base58`,
/// in ShortCode, 4 confusing characters `0` `O` `I` `l` are removed from 10 numbers
/// and 26 characters, forming a private base32 scheme.

/// Coincidentally, log(32, 29334498) is approximately equal to `4.96`, so using
/// 5-bit base32 can make good use of space, so any valid klotski layout can be
/// represented by a 5-bit length code. As in CommonCode, the characters here are
/// case insensitive, but uppercase is still recommended.

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

/// Compared with CommonCode, although ShortCode saves space, it completely loses
/// readability. The former can directly get the layout without the help of a computer,
/// while the latter is almost impossible to complete by the human brain.

#include <string>
#include <cstdint>
#include <ostream>
#include <utility>
#include <stdexcept>
#include "all_cases.h"
#include "common_code.h"

namespace klotski {

class CommonCode;

const uint32_t SHORT_CODE_LIMIT = 29334498;

class ShortCodeExp : public std::runtime_error {
public:
    ShortCodeExp() : std::runtime_error("invalid short code") {}
    explicit ShortCodeExp(const std::string &msg) : std::runtime_error(msg) {}
    ~ShortCodeExp() noexcept override = default;
};

/// For ShortCode, you must choose at least one mode (NORMAL or FAST) to convert, and
/// they all need a certain amount of time to warm up. `NORMAL` warms up quickly, and
/// `FAST` is slower, but the conversion speed of `FAST` is much faster than `NORMAL`.
/// When only converting a small amount of codes, please choose NORMAL mode. Otherwise
/// FAST mode should be used.

class ShortCode {
public:
    enum Mode {NORMAL, FAST};

private:
    uint32_t code_;
    ShortCode() = default; // unsafe initialize

    static Mode mode();
    static bool fast_mode_available_;
    static bool normal_mode_available_;

    static inline uint64_t fast_decode(uint32_t short_code) noexcept; // short code -> common code
    static inline uint32_t fast_encode(uint64_t common_code) noexcept; // common code -> short code
    static inline uint64_t tiny_decode(uint32_t short_code) noexcept; // short code -> common code
    static inline uint32_t tiny_encode(uint64_t common_code) noexcept; // common code -> short code

    static inline uint32_t string_decode(const std::string &short_code);
    static inline std::string string_encode(uint32_t short_code) noexcept;

public:
    /// Validity check
    bool valid() const noexcept;
    static bool check(uint32_t short_code) noexcept;

    /// ShortCode convert mode
    static void speed_up(Mode mode); // {} -> {NORMAL} -> {FAST}

    /// Operators of ShortCode
    constexpr explicit operator uint32_t() const noexcept { return code_; }
    friend std::ostream& operator<<(std::ostream &out, const ShortCode &self);

    /// Export functions
    std::string to_string() const noexcept;
    CommonCode to_common_code() const noexcept;
    constexpr uint32_t unwrap() const noexcept { return code_; }

    /// ShortCode constructors
    explicit ShortCode(uint32_t short_code);
    explicit ShortCode(std::string &&short_code);
    explicit ShortCode(CommonCode &&common_code) noexcept;
    explicit ShortCode(const std::string &short_code);
    explicit ShortCode(const CommonCode &common_code) noexcept;

    /// Static initialization
    static ShortCode create(uint32_t short_code);
    static ShortCode unsafe_create(uint32_t short_code) noexcept;

    static ShortCode from_string(std::string &&short_code);
    static ShortCode from_string(const std::string &short_code);

    static ShortCode from_common_code(uint64_t common_code);
    static ShortCode from_common_code(CommonCode &&common_code) noexcept;
    static ShortCode from_common_code(std::string &&common_code);
    static ShortCode from_common_code(const CommonCode &common_code) noexcept;
    static ShortCode from_common_code(const std::string &common_code);
};

inline bool operator==(uint32_t s1, const ShortCode &s2) noexcept { return s1 == s2.unwrap(); }
inline bool operator!=(uint32_t s1, const ShortCode &s2) noexcept { return s1 != s2.unwrap(); }
inline bool operator==(const ShortCode &s1, uint32_t s2) noexcept { return s1.unwrap() == s2; }
inline bool operator!=(const ShortCode &s1, uint32_t s2) noexcept { return s1.unwrap() != s2; }
inline bool operator==(const ShortCode &s1, const ShortCode &s2) noexcept { return s1.unwrap() == s2.unwrap(); }
inline bool operator!=(const ShortCode &s1, const ShortCode &s2) noexcept { return s1.unwrap() != s2.unwrap(); }

} // namespace klotski

/// In namespace std, let the ShortCode implement the standard hash function.
template<>
struct std::hash<klotski::ShortCode> {
    inline std::size_t operator()(const klotski::ShortCode &short_code) const {
        return static_cast<std::size_t>(short_code.unwrap());
    }
};
