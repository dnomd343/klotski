/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

#include <mover/mover.h>
#include <common_code/common_code.h>

#include "py_ffi/short_code.h"

// TODO: add `copy` and `pickle` support

namespace klotski::ffi {

class PyLayout {
public:
    PyLayout() = delete;

    // ------------------------------------------------------------------------------------- //

    /// Construct from origin u64 value.
    explicit PyLayout(uint64_t code);

    /// Construct from origin string form.
    explicit PyLayout(std::string_view code);

    /// Construct from PyShortCode object.
    explicit PyLayout(PyShortCode code) noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Verify CommonCode in u64 form.
    static bool check(uint64_t code) noexcept;

    /// Verify CommonCode in string form.
    static bool check(std::string_view code) noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Get original value.
    [[nodiscard]] uint64_t value() const noexcept;

    /// Convert Layout as ShortCode.
    [[nodiscard]] PyShortCode short_code() const noexcept;

    /// Convert Layout as string form.
    [[nodiscard]] std::string string(bool shorten) const noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Wrapper of `__str__` method in Python.
    static std::string str(PyLayout code) noexcept;

    /// Wrapper of `__repr__` method in Python.
    static std::string repr(PyLayout code) noexcept;

    // ------------------------------------------------------------------------------------- //

    [[nodiscard]] std::vector<PyLayout> next_cases() const noexcept;

    // ------------------------------------------------------------------------------------- //

private:
    codec::CommonCode code_;
};

static_assert(std::is_trivially_copyable_v<PyLayout>);
static_assert(sizeof(PyLayout) == sizeof(codec::CommonCode));

// ----------------------------------------------------------------------------------------- //

constexpr auto operator==(const PyLayout &lhs, const uint64_t rhs) {
    return lhs.value() == rhs;
}

constexpr auto operator<=>(const PyLayout &lhs, const uint64_t rhs) {
    return lhs.value() <=> rhs;
}

constexpr auto operator==(const PyLayout &lhs, const PyLayout &rhs) {
    return lhs.value() == rhs.value();
}

constexpr auto operator<=>(const PyLayout &lhs, const PyLayout &rhs) {
    return lhs.value() <=> rhs.value();
}

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::ffi

template <>
struct std::hash<klotski::ffi::PyLayout> {
    size_t operator()(const klotski::ffi::PyLayout &layout) const noexcept {
        return std::hash<uint64_t>{}(layout.value());
    }
};
