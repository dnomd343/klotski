/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

#include <variant>
#include <ranges/ranges.h>

#include "py_ffi/common_code.h"

namespace klotski::ffi {

using cases::RangesUnion;

class PyCasesIter {
public:
    /// Construct from RangesUnion reference.
    explicit PyCasesIter(const RangesUnion &data);

    /// Get the next CommonCode or throw a stop_iteration exception.
    PyCommonCode next();

private:
    uint8_t head_ {0};
    uint32_t index_ {0};
    const RangesUnion &data_;
};

class PyCases {
public:
    PyCases() = delete;

    // TODO: add `all_cases` interface

    // ------------------------------------------------------------------------------------- //

    /// Constructing from r-value.
    static PyCases from(RangesUnion &&data) noexcept;

    /// Constructing from longer-lived reference.
    static PyCases from_ref(const RangesUnion &data) noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Get the number of cases.
    [[nodiscard]] size_t size() const noexcept;

    /// Get CommonCode iterator of cases.
    [[nodiscard]] PyCasesIter codes() const noexcept;

    /// Get the CommonCode of the specified index.
    [[nodiscard]] PyCommonCode at(size_t index) const; // TODO: allow `-1` index

    // ------------------------------------------------------------------------------------- //

    /// Wrapper of `__repr__` method in Python.
    static std::string repr(const PyCases &cases) noexcept;

    /// Compare the cases contents of two PyCases.
    friend constexpr auto operator==(const PyCases &lhs, const PyCases &rhs);

    // ------------------------------------------------------------------------------------- //

private:
    explicit PyCases(RangesUnion &&data);
    explicit PyCases(const RangesUnion &data);

    // ------------------------------------------------------------------------------------- //

    /// Get const reference of cases data.
    [[nodiscard]] const RangesUnion& data_ref() const noexcept;

    /// Stores actual cases data or its references.
    std::variant<RangesUnion, std::reference_wrapper<const RangesUnion>> data_;

    // ------------------------------------------------------------------------------------- //
};

constexpr auto operator==(const PyCases &lhs, const PyCases &rhs) {
    return lhs.data_ref() == rhs.data_ref();
}

} // namespace klotski::ffi
