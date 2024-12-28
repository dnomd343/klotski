/// Klotski Engine by Dnomd343 @2024

/// RawCode is an uncompressed klotski coding scheme, which is used for program
/// calculation. It encodes the `5x4` chessboard as 0 ~ 19, and using 3-bit to
/// represent each position, occupying a total of 60-bit, and store them in a
/// `uint64_t` variable. In addition, the upper 4-bit of RawCode are reserved
/// and must be filled with `0`.
///
///   00 01 02 03
///   04 05 06 07    fill   20 slots
///   08 09 10 11    0000   (19) (18) (17) (16) ... (03) (02) (01) (00)
///   12 13 14 15    (4b) + (3b) * 20 => 64-bit
///   16 17 18 19

/// As we all know, 3-bit can represent 8 states. The upper-left corner of the
/// four blocks corresponds to 4 of them, and 2 more states are needed to mark
/// spaces and fills. The remaining 2 states are reserved for now.
///
///   ┌──────────────┬───────────────────┐
///   │ 000 -> space │ 100 -> 2x2        │
///   │ 001 -> 1x2   │ 101 -> [reserved] │
///   │ 010 -> 2x1   │ 110 -> [reserved] │
///   │ 011 -> 1x1   │ 111 -> fill       │
///   └──────────────┴───────────────────┘
///
/// Here, space is defined as `000` and fill is defined as `111`, which will
/// facilitate the execution of bit operations of the movement algorithm. Other
/// block definitions will not affect the efficiency of the algorithm.

/// -------------------------------------------------------------------------------------------------- ///
///   Eg1:                                                                                             ///
///     ┏━┳━━━┳━┓                                                                                      ///
///     ┃ ┃   ┃ ┃  2x1 2x2 ... 2x1    010 100 111 010                                                  ///
///     ┣━╋━━━╋━┫  ... ... ... ...    111 111 111 111                                                  ///
///     ┃ ┣━┳━┫ ┃  2x1 1x2 ... 2x1    010 001 111 010                                                  ///
///     ┣━╋━┻━╋━┫  ... 1x1 1x1 ...    111 011 011 111                                                  ///
///     ┗━┛   ┗━┛  1x1 0x0 0x0 1x1    011 000 000 011                                                  ///
///                                                                                                    ///
///        |  19  18  17  16 |  15  14  13  12 |  11  10  09  08 |  07  06  05  04 |  03  02  01  00   ///
///   0000 | 011 000 000 011 | 111 011 011 111 | 010 111 001 010 | 111 111 111 111 | 010 111 100 010   ///
///   0000 |  0110 0000 0011 |  1110 1101 1111 |  0101 1100 1010 |  1111 1111 1111 |  0101 1110 0010   ///
///      0       6    0    3       E    D    F       5    C    A       F    F    F       5    E    2   ///
///                                                                                                    ///
///   RawCode => 0x0603'EDF5'CAFF'F5E2                                                                 ///
/// -------------------------------------------------------------------------------------------------- ///

/// -------------------------------------------------------------------------------------------------- ///
///   Eg2:                                                                                             ///
///     ┏━┳━┳━┳━┓                                                                                      ///
///     ┣━┻━╋━┫ ┃  1x1 1x1 1x1 2x1    011 011 011 010                                                  ///
///     ┃   ┃ ┣━┫  2x2 ... 2x1 ...    100 111 010 111                                                  ///
///     ┗━┳━┻━┫ ┃  ... ... ... 2x1    111 111 111 010                                                  ///
///       ┣━┳━┻━┫  0x0 1x2 ... ...    000 001 111 111                                                  ///
///       ┗━┻━━━┛  0x0 1x1 1x2 ...    000 011 001 111                                                  ///
///                                                                                                    ///
///        |  19  18  17  16 |  15  14  13  12 |  11  10  09  08 |  07  06  05  04 |  03  02  01  00   ///
///   0000 | 111 001 011 000 | 111 111 001 000 | 010 111 111 111 | 111 010 111 100 | 010 011 011 011   ///
///   0000 |  1110 0101 1000 |  1111 1100 1000 |  0101 1111 1111 |  1110 1011 1100 |  0100 1101 1011   ///
///      0       E    5    8       F    C    8       5    F    F       E    B    C       4    D    B   ///
///                                                                                                    ///
///   RawCode => 0x0E58'FC85'FFEB'C4DB                                                                 ///
/// -------------------------------------------------------------------------------------------------- ///

#pragma once

#include <string>
#include <cstdint>
#include <optional>

#include "utils/utility.h"
#include "common_code/common_code_fwd.h"

namespace klotski::codec {

class RawCode {
public:
    // ------------------------------------------------------------------------------------- //

    RawCode() = delete;

    /// Construct RawCode from CommonCode.
    explicit constexpr RawCode(CommonCode common_code);

    /// Create RawCode without any check.
    static constexpr RawCode unsafe_create(uint64_t raw_code);

    /// Create RawCode with validity check.
    static constexpr std::optional<RawCode> create(uint64_t raw_code);

    // ------------------------------------------------------------------------------------- //

    /// Explicit conversion to u64 code.
    explicit constexpr operator uint64_t() const;

    /// Check the validity of the original RawCode.
    static constexpr bool check(uint64_t raw_code);

#ifndef KLSK_NDEBUG
    /// Output string encoding of RawCode only for debug.
    friend std::ostream& operator<<(std::ostream &out, RawCode self);
#endif

    // ------------------------------------------------------------------------------------- //

    /// Get the original u64 code.
    [[nodiscard]] constexpr uint64_t unwrap() const;

    /// Convert RawCode to CommonCode.
    [[nodiscard]] constexpr CommonCode to_common_code() const;

    // ------------------------------------------------------------------------------------- //

    /// Create RawCode from CommonCode.
    static constexpr RawCode from_common_code(CommonCode common_code);

    /// Create RawCode from CommonCode in u64.
    static constexpr std::optional<RawCode> from_common_code(uint64_t common_code);

    /// Create RawCode from CommonCode in string form.
    static std::optional<RawCode> from_common_code(std::string_view common_code);

    // ------------------------------------------------------------------------------------- //

    /// Whether the layout is vertically symmetrical.
    [[nodiscard]] constexpr bool is_vertical_mirror() const;

    /// Whether the layout is horizontally symmetrical.
    [[nodiscard]] constexpr bool is_horizontal_mirror() const;

    /// Calculate the vertically symmetrical klotski layout.
    [[nodiscard]] constexpr RawCode to_vertical_mirror() const;

    /// Calculate the horizontally symmetrical klotski layout.
    [[nodiscard]] constexpr RawCode to_horizontal_mirror() const;

    // ------------------------------------------------------------------------------------- //

    /// Compare RawCode with u64 value.
    friend constexpr auto operator==(const RawCode &lhs, uint64_t rhs);
    friend constexpr auto operator<=>(const RawCode &lhs, uint64_t rhs);

    /// Compare the original values of two RawCodes.
    friend constexpr auto operator==(const RawCode &lhs, const RawCode &rhs);
    friend constexpr auto operator<=>(const RawCode &lhs, const RawCode &rhs);

    // ------------------------------------------------------------------------------------- //

private:
    uint64_t code_;

    // ------------------------------------------------------------------------------------- //

    /// Compact RawCode as CommonCode.
    static constexpr uint64_t compact(uint64_t raw_code);

    /// Extract CommonCode as RawCode.
    static constexpr uint64_t extract(uint64_t common_code);

    // ------------------------------------------------------------------------------------- //

    /// Check the horizontally symmetrical.
    static constexpr bool check_mirror(uint64_t raw_code);

    /// Get the vertically symmetrical layout.
    static constexpr uint64_t get_vertical_mirror(uint64_t raw_code);

    /// Get the horizontally symmetrical layout.
    static constexpr uint64_t get_horizontal_mirror(uint64_t raw_code);

    // ------------------------------------------------------------------------------------- //
};

static_assert(sizeof(RawCode) == 8);
static_assert(is_compact_layout_v<RawCode>);
static_assert(std::is_standard_layout_v<RawCode>);
static_assert(std::is_trivially_copyable_v<RawCode>);

} // namespace klotski::codec

#include "internal/raw_code.inl"
#include "internal/convert.inl"
#include "internal/mirror.inl"
#include "internal/check.inl"

template <>
struct std::hash<klotski::codec::RawCode> {
    constexpr std::size_t operator()(const klotski::codec::RawCode &r) const noexcept {
        return std::hash<uint64_t>{}(r.unwrap());
    }
};
