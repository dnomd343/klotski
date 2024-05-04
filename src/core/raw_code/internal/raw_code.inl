#pragma once

#include <bit>

#include "common_code/common_code.h"

namespace klotski::codec {

// ------------------------------------------------------------------------------------- //

inline RawCode::RawCode(const CommonCode common_code) {
    code_ = extract(common_code.unwrap());
}

inline RawCode RawCode::unsafe_create(const uint64_t raw_code) {
    return std::bit_cast<RawCode>(raw_code); // init directly
}

inline std::optional<RawCode> RawCode::create(const uint64_t raw_code) {
    if (!check(raw_code)) {
        return std::nullopt; // invalid raw code
    }
    return unsafe_create(raw_code);
}

// ------------------------------------------------------------------------------------- //

inline RawCode::operator uint64_t() const {
    return code_;
}

inline uint64_t RawCode::unwrap() const {
    return code_;
}

inline CommonCode RawCode::to_common_code() const {
    return CommonCode::unsafe_create(compact(code_));
}

// ------------------------------------------------------------------------------------- //

inline RawCode RawCode::from_common_code(const CommonCode common_code) {
    return common_code.to_raw_code();
}

inline std::optional<RawCode> RawCode::from_common_code(const uint64_t common_code) {
    const auto convert = [](const CommonCode code) {
        return code.to_raw_code();
    };
    return CommonCode::create(common_code).transform(convert);
}

inline std::optional<RawCode> RawCode::from_common_code(const std::string &common_code) {
    const auto convert = [](const CommonCode code) {
        return code.to_raw_code();
    };
    return CommonCode::from_string(common_code).transform(convert);
}

// ------------------------------------------------------------------------------------- //

inline bool RawCode::is_vertical_mirror() const {
    return check_vertical_mirror(code_);
}

inline bool RawCode::is_horizontal_mirror() const {
    return check_horizontal_mirror(code_);
}

inline RawCode RawCode::to_vertical_mirror() const {
    return unsafe_create(get_vertical_mirror(code_));
}

inline RawCode RawCode::to_horizontal_mirror() const {
    return unsafe_create(get_horizontal_mirror(code_));
}

inline bool RawCode::is_vertical_mirror(const RawCode raw_code) const {
    return raw_code.code_ == get_vertical_mirror(code_);
}

inline bool RawCode::is_horizontal_mirror(const RawCode raw_code) const {
    return raw_code.code_ == get_horizontal_mirror(code_);
}

// ------------------------------------------------------------------------------------- //

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

// ------------------------------------------------------------------------------------- //

} // namespace klotski::codec
