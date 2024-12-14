/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

#include <mover/mover.h>
#include <common_code/common_code.h>

#include "py_ffi/short_code.h"

// TODO: maybe using `PyLayout` instead of `PyCommonCode`

namespace klotski::ffi {

class PyCommonCode {
public:
    PyCommonCode() = delete;

    // ------------------------------------------------------------------------------------- //

    /// Construct from origin u64 value.
    explicit PyCommonCode(uint64_t code);

    /// Construct from origin string form.
    explicit PyCommonCode(std::string_view code);

    /// Construct from PyShortCode object.
    explicit PyCommonCode(PyShortCode code) noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Verify CommonCode in u64 form.
    static bool check(uint64_t code) noexcept;

    /// Verify CommonCode in string form.
    static bool check(std::string_view code) noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Get original value.
    [[nodiscard]] uint64_t value() const noexcept;

    /// Convert CommonCode as ShortCode.
    [[nodiscard]] PyShortCode short_code() const noexcept;

    /// Convert CommonCode as string form.
    [[nodiscard]] std::string string(bool shorten) const noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Wrapper of `__str__` method in Python.
    static std::string str(PyCommonCode code) noexcept;

    /// Wrapper of `__repr__` method in Python.
    static std::string repr(PyCommonCode code) noexcept;

    // ------------------------------------------------------------------------------------- //

    [[nodiscard]] std::vector<PyCommonCode> next_cases() const noexcept;

    // ------------------------------------------------------------------------------------- //

private:
    codec::CommonCode code_;
};

static_assert(std::is_trivially_copyable_v<PyCommonCode>);
static_assert(sizeof(PyCommonCode) == sizeof(codec::CommonCode));

// ----------------------------------------------------------------------------------------- //

constexpr auto operator==(const PyCommonCode &lhs, const uint64_t rhs) {
    return lhs.value() == rhs;
}

constexpr auto operator<=>(const PyCommonCode &lhs, const uint64_t rhs) {
    return lhs.value() <=> rhs;
}

constexpr auto operator==(const PyCommonCode &lhs, const PyCommonCode &rhs) {
    return lhs.value() == rhs.value();
}

constexpr auto operator<=>(const PyCommonCode &lhs, const PyCommonCode &rhs) {
    return lhs.value() <=> rhs.value();
}

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::ffi

template <>
struct std::hash<klotski::ffi::PyCommonCode> {
    size_t operator()(const klotski::ffi::PyCommonCode &code) const noexcept {
        return std::hash<uint64_t>{}(code.value());
    }
};
