#pragma once

/// RawCode is an uncompressed coding scheme, which is used for program calculation. It
/// encodes a `5x4` chessboard as `0 ~ 19`, and uses 3-bit to represent each position,
/// occupying a total of 60-bit, and stored in a `uint64_t` variable. Among them, the
/// upper 4-bit are reserved and filled with `0`.
///
///   00 01 02 03
///   04 05 06 07    fill   20 slots
///   08 09 10 11    0000   (19) (18) (17) (16) ... (03) (02) (01) (00)
///   12 13 14 15    (4b) + (3b) * 20 => 64-bit
///   16 17 18 19
///
///   Eg1:
///     % # # %    2x1 2x2 ... 2x1    010 100 111 010
///     % # # %    ... ... ... ...    111 111 111 111
///     @ $ $ @    2x1 1x2 ... 2x1    010 001 111 010
///     @ & * @    ... 1x1 1x1 ...    111 011 011 111
///     *     &    1x1 0x0 0x0 1x1    011 000 000 011
///
///          |  19  18  17  16 |  15  14  13  12 |  11  10  09  08 |  07  06  05  04 |  03  02  01  00
///     0000 | 011 000 000 011 | 111 011 011 111 | 010 111 001 010 | 111 111 111 111 | 010 111 100 010
///     0000 |  0110 0000 0011 |  1110 1101 1111 |  0101 1100 1010 |  1111 1111 1111 |  0101 1110 0010
///        0       6    0    3       E    D    F       5    C    A       F    F    F       5    E    2
///     => 0x0603'EDF5'CAFF'F5E2
///
///   Eg2:
///     * @ & %    1x1 1x1 1x1 2x1    011 011 011 010
///     # # $ %    2x2 ... 2x1 ...    100 111 010 111
///     # # $ ^    ... ... ... 2x1    111 111 111 010
///       ~ ~ ^    0x0 1x2 ... ...    000 001 111 111
///       @ % %    0x0 1x1 1x2 ...    000 011 001 111
///
///          |  19  18  17  16 |  15  14  13  12 |  11  10  09  08 |  07  06  05  04 |  03  02  01  00
///     0000 | 111 001 011 000 | 111 111 001 000 | 010 111 111 111 | 111 010 111 100 | 010 011 011 011
///     0000 |  1110 0101 1000 |  1111 1100 1000 |  0101 1111 1111 |  1110 1011 1100 |  0100 1101 1011
///        0       E    5    8       F    C    8       5    F    F       E    B    C       4    D    B
///     => 0x0E58'FC85'FFEB'C4DB

#include <string>
#include <vector>
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include "common_code.h"

namespace klotski {

class RawCode;
class CommonCode;

typedef std::vector<RawCode> RawCodes;
typedef std::vector<CommonCode> CommonCodes;

class RawCodeExp : public std::runtime_error {
public:
    RawCodeExp() : std::runtime_error("invalid raw code") {}
    explicit RawCodeExp(const std::string &msg) : std::runtime_error(msg) {}
    ~RawCodeExp() noexcept override = default;
};

class RawCode {
    uint64_t code_;
    RawCode() = default; // unsafe initialize

    static inline uint64_t compact(uint64_t raw_code) noexcept; // raw code -> common code
    static inline uint64_t extract(uint64_t common_code) noexcept; // common code -> raw code

    static inline uint64_t get_vertical_mirror(uint64_t raw_code) noexcept;
    static inline uint64_t get_horizontal_mirror(uint64_t raw_code) noexcept;

    static inline bool check_vertical_mirror(uint64_t raw_code) noexcept;
    static inline bool check_horizontal_mirror(uint64_t raw_code) noexcept;

public:
    /// Validity check
    static bool check(uint64_t raw_code) noexcept;
    bool valid() const noexcept { return check(code_); }

    /// Operators of RawCode
    constexpr explicit operator uint64_t() const noexcept { return code_; }
    friend std::ostream& operator<<(std::ostream &out, const RawCode &self);

    /// Export functions
    CommonCode to_common_code() const noexcept;
    constexpr uint64_t unwrap() const noexcept { return code_; }

    /// RawCode constructors
    explicit RawCode(uint64_t raw_code);
    explicit RawCode(CommonCode &&common_code) noexcept;
    explicit RawCode(const CommonCode &common_code) noexcept;

    /// RawCode initializations
    static RawCode create(uint64_t raw_code);
    static RawCode unsafe_create(uint64_t raw_code) noexcept;

    static RawCode from_common_code(uint64_t common_code);
    static RawCode from_common_code(CommonCode &&common_code) noexcept;
    static RawCode from_common_code(std::string &&common_code);
    static RawCode from_common_code(const CommonCode &common_code) noexcept;
    static RawCode from_common_code(const std::string &common_code);

    /// Batch conversion
    static CommonCodes convert(const RawCodes &raw_codes) noexcept;

    /// Mirror functions
    RawCode to_vertical_mirror() const noexcept;
    RawCode to_horizontal_mirror() const noexcept;

    bool is_vertical_mirror() const noexcept; // whether vertically symmetrical
    bool is_horizontal_mirror() const noexcept; // whether horizontally symmetrical

    bool is_vertical_mirror(RawCode &&raw_code) const noexcept; // whether vertically symmetric to another
    bool is_vertical_mirror(const RawCode &raw_code) const noexcept;
    bool is_horizontal_mirror(RawCode &&raw_code) const noexcept; // whether horizontally symmetric to another
    bool is_horizontal_mirror(const RawCode &raw_code) const noexcept;
};

/// RawCode create
inline RawCode RawCode::create(uint64_t raw_code) {
    return RawCode(raw_code); // with check
}

/// RawCode create without check
inline RawCode RawCode::unsafe_create(uint64_t raw_code) noexcept {
    return *(RawCode*)&raw_code; // init directly
}

/// Compare implements
inline bool operator==(uint64_t r1, const RawCode &r2) noexcept { return r1 == r2.unwrap(); }
inline bool operator==(const RawCode &r1, uint64_t r2) noexcept { return r1.unwrap() == r2; }
inline bool operator==(const RawCode &r1, const RawCode &r2) noexcept { return r1.unwrap() == r2.unwrap(); }

inline bool operator!=(uint64_t r1, const RawCode &r2) noexcept { return !(r1 == r2); }
inline bool operator!=(const RawCode &r1, uint64_t r2) noexcept { return !(r1 == r2); }
inline bool operator!=(const RawCode &r1, const RawCode &r2) noexcept { return !(r1 == r2); }

} // namespace klotski

/// In namespace std, let the RawCode implement the standard hash function.
template<>
struct std::hash<klotski::RawCode> {
    inline std::size_t operator()(const klotski::RawCode &raw_code) const {
        return static_cast<std::size_t>(raw_code.unwrap());
    }
};
