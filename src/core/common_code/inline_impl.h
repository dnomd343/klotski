#pragma once

namespace klotski {
namespace codec {

/// Get the original 64-bit code.
inline uint64_t CommonCode::unwrap() const noexcept {
    return code_;
}

/// Implicit conversion to 64-bit code.
inline CommonCode::operator uint64_t() const noexcept {
    return code_;
}

/// Equality comparison between CommonCode and numbers.
inline bool operator==(CommonCode c1, uint64_t c2) noexcept {
    return c1.unwrap() == c2;
}

/// CommonCode equal comparison implement.
inline bool operator==(CommonCode c1, CommonCode c2) noexcept {
    return c1.unwrap() == c2.unwrap();
}

/// CommonCode less than comparison implement.
inline bool operator<(CommonCode c1, CommonCode c2) noexcept {
    return c1.unwrap() < c2.unwrap();
}

/// CommonCode greater than comparison implement.
inline bool operator>(CommonCode c1, CommonCode c2) noexcept {
    return c1.unwrap() > c2.unwrap();
}

/// CommonCode create without any check.
inline CommonCode CommonCode::unsafe_create(uint64_t common_code) noexcept {
    return *reinterpret_cast<CommonCode*>(&common_code); // init directly
}

/// CommonCode create with valid check.
inline std::optional<CommonCode> CommonCode::create(uint64_t common_code) noexcept {
    if (!CommonCode::check(common_code)) {
        return std::nullopt; // invalid common code
    }
    return CommonCode::unsafe_create(common_code);
}

/// Output string encoding of CommonCode.
inline std::ostream& operator<<(std::ostream &out, CommonCode self) {
    out << CommonCode::string_encode(self.code_);
    return out;
}

} // namespace codec
} // namespace klotski
