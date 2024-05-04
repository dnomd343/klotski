#pragma once

#include <bit>

namespace klotski::codec {

// ------------------------------------------------------------------------------------- //

inline uint64_t CommonCode::unwrap() const {
    return code_;
}

inline CommonCode::operator uint64_t() const {
    return code_;
}

inline std::ostream& operator<<(std::ostream &out, const CommonCode self) {
    out << CommonCode::string_encode(self.code_);
    return out;
}

// ------------------------------------------------------------------------------------- //

constexpr auto operator==(const CommonCode &lhs, const uint64_t rhs) {
    return lhs.code_ == rhs;
}

constexpr auto operator<=>(const CommonCode &lhs, const uint64_t rhs) {
    return lhs.code_ <=> rhs;
}

constexpr auto operator==(const CommonCode &lhs, const CommonCode &rhs) {
    return lhs.code_ == rhs.code_;
}

constexpr auto operator<=>(const CommonCode &lhs, const CommonCode &rhs) {
    return lhs.code_ <=> rhs.code_;
}

// ------------------------------------------------------------------------------------- //

inline CommonCode CommonCode::unsafe_create(const uint64_t common_code) {
    return std::bit_cast<CommonCode>(common_code); // init directly
}

inline std::optional<CommonCode> CommonCode::create(const uint64_t common_code) {
    if (!check(common_code)) {
        return std::nullopt; // invalid common code
    }
    return unsafe_create(common_code);
}

// ------------------------------------------------------------------------------------- //

inline std::string CommonCode::to_string(const bool shorten) const {
    if (!shorten) {
        return string_encode(code_); // with full length
    }
    return string_encode_shorten(code_); // without trailing zero
}

inline std::optional<CommonCode> CommonCode::from_string(const std::string &common_code) {
    auto construct = [](const uint64_t code) {
        return unsafe_create(code);
    };
    return string_decode(common_code).transform(construct);
}

// ------------------------------------------------------------------------------------- //

} // namespace klotski::codec
