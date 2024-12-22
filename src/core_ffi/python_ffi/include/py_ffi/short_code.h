/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

#include <short_code/short_code.h>

namespace klotski::ffi {

class PyLayout;

class PyShortCode {
public:
    PyShortCode() = delete;

    // ------------------------------------------------------------------------------------- //

    /// Construct from origin u32 value.
    explicit PyShortCode(uint32_t code);

    /// Construct from origin string form.
    explicit PyShortCode(std::string_view code);

    /// Construct from PyLayout object.
    explicit PyShortCode(PyLayout code) noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Verify ShortCode in u32 form.
    static bool check(uint32_t code) noexcept;

    /// Verify ShortCode in string form.
    static bool check(std::string_view code) noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Get original value.
    [[nodiscard]] uint32_t value() const noexcept;

    /// Convert ShortCode as Layout.
    [[nodiscard]] PyLayout layout() const noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Wrapper of `__str__` method in Python.
    static std::string str(PyShortCode code) noexcept;

    /// Wrapper of `__repr__` method in Python.
    static std::string repr(PyShortCode code) noexcept;

    // ------------------------------------------------------------------------------------- //

private:
    codec::ShortCode code_;
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

template <>
struct std::hash<klotski::ffi::PyShortCode> {
    size_t operator()(const klotski::ffi::PyShortCode &code) const noexcept {
        return std::hash<uint32_t>{}(code.value());
    }
};
