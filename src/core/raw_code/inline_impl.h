#pragma once

namespace klotski {
namespace codec {

/// Get the original 64-bit code.
inline uint64_t RawCode::unwrap() const noexcept {
    return code_;
}

/// Implicit conversion to 64-bit code.
inline RawCode::operator uint64_t() const noexcept {
    return code_;
}

/// Equality comparison between RawCode and numbers.
inline bool operator==(RawCode r1, uint64_t r2) noexcept {
    return r1.unwrap() == r2;
}

/// RawCode equal comparison implement.
inline bool operator==(RawCode r1, RawCode r2) noexcept {
    return r1.unwrap() == r2.unwrap();
}

/// RawCode create without any check.
inline RawCode RawCode::unsafe_create(uint64_t raw_code) noexcept {
    return *reinterpret_cast<RawCode*>(&raw_code); // init directly
}

/// RawCode create with valid check.
inline std::optional<RawCode> RawCode::create(uint64_t raw_code) noexcept {
    if (!RawCode::check(raw_code)) {
        return std::nullopt; // invalid raw code
    }
    return RawCode::unsafe_create(raw_code);
}

/// Calculate vertically symmetrical case.
inline RawCode RawCode::to_vertical_mirror() const noexcept {
    return RawCode::unsafe_create(get_vertical_mirror(code_));
}

/// Calculate horizontally symmetrical case.
inline RawCode RawCode::to_horizontal_mirror() const noexcept {
    return RawCode::unsafe_create(get_horizontal_mirror(code_));
}

/// Determine whether the case is vertically symmetrical.
inline bool RawCode::is_vertical_mirror() const noexcept {
    return check_vertical_mirror(code_);
}

/// Determine whether the case is horizontally symmetrical.
inline bool RawCode::is_horizontal_mirror() const noexcept {
    return check_horizontal_mirror(code_);
}

/// Determine whether two cases are vertically symmetrical to each other.
inline bool RawCode::is_vertical_mirror(RawCode raw_code) const noexcept {
    return raw_code.code_ == get_vertical_mirror(code_);
}

/// Determine whether two cases are horizontally symmetrical to each other.
inline bool RawCode::is_horizontal_mirror(RawCode raw_code) const noexcept {
    return raw_code.code_ == get_horizontal_mirror(code_);
}

} // namespace codec
} // namespace klotski
