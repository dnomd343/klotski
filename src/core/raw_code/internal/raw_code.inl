#pragma once

#include <bit>

namespace klotski::codec {

// ------------------------------------------------------------------------------------- //

inline uint64_t RawCode::unwrap() const {
    return code_;
}

inline RawCode::operator uint64_t() const {
    return code_;
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

} // namespace klotski::codec
