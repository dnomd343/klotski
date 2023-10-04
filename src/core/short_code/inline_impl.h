#pragma once

namespace klotski {
namespace codec {

/// Get the original 32-bit code.
inline uint32_t ShortCode::unwrap() const noexcept {
    return code_;
}

/// Implicit conversion to 32-bit code.
inline ShortCode::operator uint32_t() const noexcept {
    return code_;
}

/// Equality comparison between ShortCode and numbers.
inline bool operator==(ShortCode s1, uint32_t s2) noexcept {
    return s1.unwrap() == s2;
}

/// ShortCode equal comparison implement.
inline bool operator==(ShortCode s1, ShortCode s2) noexcept {
    return s1.unwrap() == s2.unwrap();
}

/// ShortCode less than comparison implement.
inline bool operator<(ShortCode s1, ShortCode s2) noexcept {
    return s1.unwrap() < s2.unwrap();
}

/// ShortCode greater than comparison implement.
inline bool operator>(ShortCode s1, ShortCode s2) noexcept {
    return s1.unwrap() > s2.unwrap();
}

/// ShortCode create without any check.
inline ShortCode ShortCode::unsafe_create(uint32_t short_code) noexcept {
    return *reinterpret_cast<ShortCode*>(&short_code); // init directly
}

/// ShortCode create with valid check.
inline std::optional<ShortCode> ShortCode::create(uint32_t short_code) noexcept {
    if (!ShortCode::check(short_code)) {
        return std::nullopt; // invalid short code
    }
    return ShortCode::unsafe_create(short_code);
}

/// Output string encoding of ShortCode.
inline std::ostream& operator<<(std::ostream &out, ShortCode self) {
    out << ShortCode::string_encode(self.code_);
    return out;
}

} // namespace codec
} // namespace klotski
