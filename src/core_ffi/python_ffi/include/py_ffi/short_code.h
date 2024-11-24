/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

#include <short_code/short_code.h>

#include "py_ffi/common_code.h"

namespace klotski::ffi {

using codec::ShortCode;

class PyCommonCode;

class PyShortCode {
public:
    PyShortCode() = delete;

    // ------------------------------------------------------------------------------------- //

    /// Construct from origin u32 value.
    explicit PyShortCode(uint32_t code);

    /// Construct from origin string form.
    explicit PyShortCode(std::string_view code);

    /// Construct from PyCommonCode object.
    explicit PyShortCode(PyCommonCode code) noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Verify ShortCode in u32 form.
    static bool check(uint32_t code) noexcept;

    /// Verify ShortCode in string form.
    static bool check(std::string_view code) noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Get original value.
    [[nodiscard]] uint32_t value() const noexcept;

    /// Convert ShortCode as CommonCode.
    [[nodiscard]] PyCommonCode common_code() const noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Wrapper of `__str__` method in Python.
    static std::string str(PyShortCode code) noexcept;

    /// Wrapper of `__repr__` method in Python.
    static std::string repr(PyShortCode code) noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Build conversion index for ShortCode.
    static void speed_up(const bool fast_mode) { // TODO: move to `SpeedUp`
        ShortCode::speed_up(fast_mode);
    }

private:
    ShortCode code_;
};

// ----------------------------------------------------------------------------------------- //

constexpr auto operator==(const PyShortCode &lhs, const uint32_t rhs) {
    return lhs.value() == rhs;
}

constexpr auto operator<=>(const PyShortCode &lhs, const uint32_t rhs) {
    return lhs.value() <=> rhs;
}

constexpr auto operator==(const PyShortCode &lhs, const PyShortCode &rhs) {
    return lhs.value() == rhs.value();
}

constexpr auto operator<=>(const PyShortCode &lhs, const PyShortCode &rhs) {
    return lhs.value() <=> rhs.value();
}

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::ffi

// ----------------------------------------------------------------------------------------- //

namespace std {

template <>
struct hash<klotski::ffi::PyShortCode> {
    size_t operator()(const klotski::ffi::PyShortCode &short_code) const noexcept {
        return std::hash<uint32_t>{}(short_code.value());
    }
};

} // namespace std

// ----------------------------------------------------------------------------------------- //
