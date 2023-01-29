#pragma once

/// ShortCode is a high-compression encoding scheme based on CommonCode. Since there
/// are a total of 29334498 valid klotski layouts, arrange their CommonCodes from
/// small to large (36-bits positive integers), and use the index as the ShortCode.

/// Therefore, the valid value of ShortCode is [0, 29334498), stored in `uint32_t`.
/// The goal of high compression ratio is to facilitate verbal sharing, so it is
/// necessary to represent it in a suitable string. Similar to Bitcoin's base58,
/// in ShortCode, 4 confusing characters `0` `O` `I` `l` are removed from 10 numbers
/// and 26 characters, forming a private base32 scheme.

/// Coincidentally, log(32, 29334498) is approximately equal to `4.96`, so using
/// 5-bits base32 can make good use of space, so any valid klotski layout can be
/// represented by a 5-bits length code. As in CommonCode, the characters here are
/// case insensitive, but uppercase is still recommended.

///   Eg1: 0x1A9BF0C00 ->  4091296 -> "4WVE1"
///   Eg2: 0x4FEA13400 -> 10399732 -> "AXCZN"

/// Compared with CommonCode, although ShortCode saves space, it completely loses
/// readability. The former can directly get the layout without the help of a computer,
/// while the latter is almost impossible to complete by the human brain.

#include <string>
#include <cstdint>
#include <ostream>
#include "common_code.h"

namespace klotski {
    /// import for convert interface
    class CommonCode;

    class ShortCode {
    public:
        enum Mode {NORMAL, FAST};

        bool valid() const;
        static void speed_up(enum Mode mode);
        static bool check(uint32_t short_code);

        /// Operators of ShortCode
        bool operator==(const ShortCode &short_code) const;
        constexpr explicit operator uint32_t() const { return code; }
        friend std::ostream& operator<<(std::ostream &out, const ShortCode &self);

        /// Export functions
        std::string to_string() const;
        CommonCode to_common_code() const;
        constexpr uint32_t unwrap() const { return code; }

        /// ShortCode constructors
        explicit ShortCode(uint32_t short_code);
        explicit ShortCode(const std::string &short_code);
        explicit ShortCode(const CommonCode &common_code);

        ShortCode(uint32_t short_code, enum Mode mode) : ShortCode(short_code) { speed_up(mode); }
        ShortCode(const std::string &short_code, enum Mode mode) : ShortCode(short_code) { speed_up(mode); }
        ShortCode(const CommonCode &common_code, enum Mode mode) : ShortCode(common_code) { speed_up(mode); }

        /// Rust-style initialization
        static ShortCode create(uint32_t short_code);
        static ShortCode unsafe_create(uint32_t short_code);
        static ShortCode from_string(const std::string &short_code);

        static ShortCode from_common_code(uint64_t common_code);
        static ShortCode from_common_code(const CommonCode &common_code);
        static ShortCode from_common_code(const std::string &common_code);

    private:
        uint32_t code;
        static bool fast_mode_available;
        static bool normal_mode_available;
        static const uint32_t SHORT_CODE_LIMIT = 29334498;

        ShortCode() = default; // unsafe initialize
        static enum Mode mode();
        static uint64_t fast_decode(uint32_t short_code);
        static uint32_t fast_encode(uint64_t common_code);
        static uint64_t tiny_decode(uint32_t short_code);
        static uint32_t tiny_encode(uint64_t common_code);
    };
}
