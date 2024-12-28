/// Klotski Engine by Dnomd343 @2024

/// ShortCode is a high-compression encoding scheme based on CommonCode. Since
/// there are a total of 29334498 valid klotski cases, arrange them from small
/// to large by their CommonCodes (36-bit positive integers), and use the index
/// as the ShortCode.

/// Therefore, the valid value of ShortCode is [0, 29334498), which stored in a
/// `uint32_t` variable. The goal of high compression ratio is to facilitate
/// verbal sharing, so it is necessary to represent it into a suitable string.
/// Similar to Bitcoin's base58 encoding, in ShortCode, 4 confusing characters
/// `0` `O` `I` `l` are removed from 10 numbers and 26 characters, forming a
/// private base32 scheme.

/// Coincidentally, log(32, 29334498) is approximately equal to 4.96, so using
/// 5-bit base32 can make good use of space, so any valid klotski cases can be
/// represented by a 5-bit length code. As in CommonCode, the characters here
/// are case-insensitive, but uppercase is still recommended.

/// Compared with the CommonCode, although ShortCode saves space, it completely
/// loses readability. The former can directly get the case without the help of
/// a computer, while the latter is almost impossible to complete by the human
/// brain. But anyway, ShortCode makes it easy to manually record the klotski
/// cases, either verbally or handwritten.

///                ShortCode Convert Table                ///
///   ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐   ///
///   │  00 │  01 │  02 │  03 │  04 │  05 │  06 │  07 │   ///
///   │ `1` │ `2` │ `3` │ `4` │ `5` │ `6` │ `7` │ `8` │   ///
///   ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤   ///
///   │  08 │  09 │  10 │  11 │  12 │  13 │  14 │  15 │   ///
///   │ `9` │ `A` │ `B` │ `C` │ `D` │ `E` │ `F` │ `G` │   ///
///   ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤   ///
///   │  16 │  17 │  18 │  19 │  20 │  21 │  22 │  23 │   ///
///   │ `H` │ `J` │ `K` │ `M` │ `N` │ `P` │ `Q` │ `R` │   ///
///   ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤   ///
///   │  24 │  25 │  26 │  27 │  28 │  29 │  30 │  31 │   ///
///   │ `S` │ `T` │ `U` │ `V` │ `W` │ `X` │ `Y` │ `Z` │   ///
///   └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘   ///

/// ------------------------------------------------------------------------------------------------ ///
///   Eg1:                                                                                           ///
///     ┏━┳━━━┳━┓  CommonCode = 0x1A9BF0C00 (index 4091296)                                          ///
///     ┃ ┃   ┃ ┃  ShortCode = 4091296                                                               ///
///     ┣━╋━━━╋━┫    => 3 * (32 ^ 4) + 28 * (32 ^ 3) + 27 * (32 ^ 2) + 13 * (32 ^ 1) + 0 * (32 ^ 0)  ///
///     ┃ ┣━┳━┫ ┃    => (3), (28), (27), (13), (0)                                                   ///
///     ┣━╋━┻━╋━┫    => `4`, `W`, `V`, `E`, `1`                                                      ///
///     ┗━┛   ┗━┛    => "4WVE1"                                                                      ///
/// ------------------------------------------------------------------------------------------------ ///

/// ------------------------------------------------------------------------------------------------- ///
///   Eg2:                                                                                            ///
///     ┏━┳━┳━┳━┓  CommonCode = 0x4FEA13400 (index 10399732)                                          ///
///     ┣━┻━╋━┫ ┃  ShortCode = 10399732                                                               ///
///     ┃   ┃ ┣━┫    => 9 * (32 ^ 4) + 29 * (32 ^ 3) + 11 * (32 ^ 2) + 31 * (32 ^ 1) + 20 * (32 ^ 0)  ///
///     ┗━┳━┻━┫ ┃    => (9), (29), (11), (31), (20)                                                   ///
///       ┣━┳━┻━┫    => `A`, `X`, `C`, `Z`, `N`                                                       ///
///       ┗━┻━━━┛    => "AXCZN"                                                                       ///
/// ------------------------------------------------------------------------------------------------- ///

#pragma once

#include <mutex>
#include <string>
#include <cstdint>
#include <optional>

#include "utils/utility.h"
#include "ranges/ranges_fwd.h"
#include "all_cases/all_cases_fwd.h"
#include "common_code/common_code_fwd.h"

namespace klotski::codec {

constexpr uint32_t SHORT_CODE_LIMIT = cases::ALL_CASES_NUM_;

class ShortCode {
public:
    // ------------------------------------------------------------------------------------- //

    ShortCode() = delete;

    /// Construct ShortCode from CommonCode.
    explicit ShortCode(CommonCode common_code);

    /// Create ShortCode without any check.
    static constexpr ShortCode unsafe_create(uint32_t short_code);

    /// Create ShortCode with validity check.
    static constexpr std::optional<ShortCode> create(uint32_t short_code);

    // ------------------------------------------------------------------------------------- //

    /// Build the conversion index.
    static void speed_up(bool fast_mode);

    /// Explicit conversion to u32 code.
    explicit constexpr operator uint32_t() const;

    /// Check the validity of the original ShortCode.
    static constexpr bool check(uint32_t short_code);

#ifndef KLSK_NDEBUG
    /// Output string encoding of ShortCode only for debug.
    friend std::ostream& operator<<(std::ostream &out, ShortCode self);
#endif

    // ------------------------------------------------------------------------------------- //

    /// Get the original u32 code.
    [[nodiscard]] constexpr uint32_t unwrap() const;

    /// Convert ShortCode to string form.
    [[nodiscard]] std::string to_string() const;

    /// Convert ShortCode to CommonCode.
    [[nodiscard]] CommonCode to_common_code() const;

    // ------------------------------------------------------------------------------------- //

    /// Create ShortCode from string form.
    static std::optional<ShortCode> from_string(std::string_view short_code);

    // ------------------------------------------------------------------------------------- //

    /// Create ShortCode from CommonCode.
    static ShortCode from_common_code(CommonCode common_code);

    /// Create ShortCode from CommonCode in u64.
    static std::optional<ShortCode> from_common_code(uint64_t common_code);

    /// Create ShortCode from CommonCode in string form.
    static std::optional<ShortCode> from_common_code(std::string_view common_code);

    // ------------------------------------------------------------------------------------- //

    /// Compare ShortCode with u32 value.
    friend constexpr auto operator==(const ShortCode &lhs, uint32_t rhs);
    friend constexpr auto operator<=>(const ShortCode &lhs, uint32_t rhs);

    /// Compare the original values of two ShortCodes.
    friend constexpr auto operator==(const ShortCode &lhs, const ShortCode &rhs);
    friend constexpr auto operator<=>(const ShortCode &lhs, const ShortCode &rhs);

    // ------------------------------------------------------------------------------------- //

private:
    uint32_t code_;

    // ------------------------------------------------------------------------------------- //

    /// Convert ShortCode to CommonCode based on AllCases data.
    static KLSK_INLINE uint64_t fast_decode(uint32_t short_code);

    /// Convert CommonCode to ShortCode based on AllCases data.
    static KLSK_INLINE uint32_t fast_encode(uint64_t common_code);

    /// Convert ShortCode to CommonCode based on BasicRanges data.
    static uint64_t tiny_decode(uint32_t short_code);

    /// Convert CommonCode to ShortCode based on BasicRanges data.
    static uint32_t tiny_encode(uint64_t common_code);

    // ------------------------------------------------------------------------------------- //

    /// Serialize ShortCode into 5-bit length string.
    static KLSK_INLINE std::string string_encode(uint32_t short_code);

    /// Deserialize ShortCode from string and return nullopt on error.
    static KLSK_INLINE std::optional<uint32_t> string_decode(std::string_view short_code);

    // ------------------------------------------------------------------------------------- //

    /// Whether fast mode is available.
    static inline bool fast_ {false};

    /// Mutex for protecting critical section.
    static inline std::mutex busy_ {};

    /// Static pointer to klotski AllCases data.
    static inline const cases::RangesUnion *cases_ {nullptr};

    /// Static pointer to klotski BasicRanges data.
    static inline std::atomic<const cases::Ranges*> ranges_ {nullptr};

    // ------------------------------------------------------------------------------------- //
};

static_assert(sizeof(ShortCode) == 4);
static_assert(is_compact_layout_v<ShortCode>);
static_assert(std::is_standard_layout_v<ShortCode>);
static_assert(std::is_trivially_copyable_v<ShortCode>);

} // namespace klotski::codec

#include "internal/short_code.inl"
#include "internal/serialize.inl"
#include "internal/convert.inl"

template <>
struct std::hash<klotski::codec::ShortCode> {
    constexpr std::size_t operator()(const klotski::codec::ShortCode &s) const noexcept {
        return std::hash<uint32_t>{}(s.unwrap());
    }
};
