/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

#include <mover/mover.h>
#include <common_code/common_code.h>

#include "py_ffi/short_code.h"

namespace klotski::ffi {

using codec::CommonCode;

class PyShortCode;

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

    [[nodiscard]] std::vector<PyCommonCode> next_cases() const noexcept {
        std::vector<PyCommonCode> cases;
        auto mover = mover::MaskMover([&cases](const codec::RawCode code, uint64_t) {
            cases.emplace_back(std::bit_cast<PyCommonCode>(code.to_common_code()));
        });
        mover.next_cases(code_.to_raw_code(), 0);
        return cases;
    }

    // ------------------------------------------------------------------------------------- //

private:
    CommonCode code_;
};

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

// ----------------------------------------------------------------------------------------- //

namespace std {

template <>
struct hash<klotski::ffi::PyCommonCode> {
    size_t operator()(const klotski::ffi::PyCommonCode &common_code) const noexcept {
        return std::hash<uint64_t>{}(common_code.value());
    }
};

} // namespace std

// ----------------------------------------------------------------------------------------- //
