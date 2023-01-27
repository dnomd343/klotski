#pragma once

#include <string>
#include <cstdint>
#include <ostream>
#include "raw_code.h"
#include "short_code.h"

namespace klotski {
    /// import for convert interface
    class RawCode;
    class ShortCode;

    class CommonCode {
    public:
        bool valid() const;
        static bool check(uint64_t common_code);

        /// Operators of CommonCode
        explicit operator uint64_t() const { return code; }
        bool operator==(const CommonCode &common_code) const;
        friend std::ostream& operator<<(std::ostream &out, const CommonCode &self);

        /// Export functions
        uint64_t unwrap() const;
        RawCode to_raw_code() const;
        ShortCode to_short_code() const;
        std::string to_string(bool shorten = false) const;

        /// CommonCode constructors
        explicit CommonCode(uint64_t common_code);
        explicit CommonCode(const RawCode &raw_code);
        explicit CommonCode(const ShortCode &short_code);
        explicit CommonCode(const std::string &common_code);

        /// Rust-style initialization
        static CommonCode create(uint64_t common_code);
        static CommonCode unsafe_create(uint64_t common_code);
        static CommonCode from_string(const std::string &common_code);

        static CommonCode from_raw_code(uint64_t raw_code);
        static CommonCode from_raw_code(const RawCode &raw_code);

        static CommonCode from_short_code(uint32_t short_code);
        static CommonCode from_short_code(const ShortCode &short_code);
        static CommonCode from_short_code(const std::string &short_code);

    private:
        uint64_t code;
        CommonCode() = default; // unsafe initialize
    };
}
