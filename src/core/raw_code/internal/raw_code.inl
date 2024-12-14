#pragma once

#include <format>

#include "common_code/common_code.h"

namespace klotski::codec {

// ----------------------------------------------------------------------------------------- //

constexpr RawCode::RawCode(const CommonCode common_code) {
    code_ = extract(common_code.unwrap());
}

constexpr RawCode RawCode::unsafe_create(const uint64_t raw_code) {
    return std::bit_cast<RawCode>(raw_code); // init directly
}

constexpr std::optional<RawCode> RawCode::create(const uint64_t raw_code) {
    if (!check(raw_code)) {
        return std::nullopt; // invalid raw code
    }
    return unsafe_create(raw_code);
}

// ----------------------------------------------------------------------------------------- //

constexpr RawCode::operator uint64_t() const {
    return code_;
}

constexpr uint64_t RawCode::unwrap() const {
    return code_;
}

constexpr CommonCode RawCode::to_common_code() const {
    return CommonCode::unsafe_create(compact(code_));
}

// ----------------------------------------------------------------------------------------- //

constexpr RawCode RawCode::from_common_code(const CommonCode common_code) {
    return common_code.to_raw_code();
}

constexpr std::optional<RawCode> RawCode::from_common_code(const uint64_t common_code) {
    const auto convert = [](const CommonCode code) {
        return code.to_raw_code();
    };
    return CommonCode::create(common_code).transform(convert);
}

inline std::optional<RawCode> RawCode::from_common_code(const std::string_view common_code) {
    const auto convert = [](const CommonCode code) {
        return code.to_raw_code();
    };
    return CommonCode::from_string(common_code).transform(convert);
}

// ----------------------------------------------------------------------------------------- //

constexpr bool RawCode::is_vertical_mirror() const {
    return false;
}

constexpr bool RawCode::is_horizontal_mirror() const {
    return check_mirror(code_);
}

constexpr RawCode RawCode::to_vertical_mirror() const {
    return unsafe_create(get_vertical_mirror(code_));
}

constexpr RawCode RawCode::to_horizontal_mirror() const {
    return unsafe_create(get_horizontal_mirror(code_));
}

// ----------------------------------------------------------------------------------------- //

constexpr auto operator==(const RawCode &lhs, const uint64_t rhs) {
    return lhs.code_ == rhs;
}

constexpr auto operator<=>(const RawCode &lhs, const uint64_t rhs) {
    return lhs.code_ <=> rhs;
}

constexpr auto operator==(const RawCode &lhs, const RawCode &rhs) {
    return lhs.code_ == rhs.code_;
}

constexpr auto operator<=>(const RawCode &lhs, const RawCode &rhs) {
    return lhs.code_ <=> rhs.code_;
}

// ----------------------------------------------------------------------------------------- //

#ifndef KLSK_NDEBUG
inline std::ostream& operator<<(std::ostream &out, const RawCode self) {
    auto show = [code = self.code_](const int offset) {
        constexpr auto char_map = std::to_array({
            '.', // space
            '~', '|', // 1x2 | 2x1
            '*', '@', // 1x1 | 2x2
            '?', '?', // unknown
            '+', // fill
        });
        return char_map[(code >> (offset * 3)) & 0b111];
    };

    out << std::format("{:015X}\n", self.code_);
    for (int offset = 0; offset < 20; offset += 4) {
        out << std::format("{} {} {} {}\n",
            show(offset), show(offset + 1), show(offset + 2), show(offset + 3));
    }
    return out;
}
#endif

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::codec
