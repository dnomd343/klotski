/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

#include <variant>
#include <ranges/ranges.h>

namespace klotski::ffi {

using cases::RangesUnion;

class PyLayout;

class PyCasesIter {
public:
    /// Construct from RangesUnion reference.
    explicit PyCasesIter(const RangesUnion &data) noexcept;

    /// Get the next CommonCode or throw `stop_iteration` exception.
    PyLayout next();

private:
    size_t index_ {0};
    uint64_t head_ {0};
    const RangesUnion &data_;
};

// TODO: add `copy` and `pickle` support

class PyCases {
public:
    PyCases() = delete;

    // ------------------------------------------------------------------------------------- //

    /// Constructing from r-value.
    static PyCases from(RangesUnion &&data) noexcept;

    /// Constructing from longer-lived reference.
    static PyCases from_ref(const RangesUnion &data) noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Get the number of cases.
    [[nodiscard]] size_t size() const noexcept;

    /// Get CommonCode iterator of cases.
    [[nodiscard]] PyCasesIter iter() const noexcept;

    /// Get the CommonCode of the specified index.
    [[nodiscard]] PyLayout at(int32_t index) const;

    // TODO: add `contain` interface

    // ------------------------------------------------------------------------------------- //

    /// Export all klotski cases.
    static PyCases all_cases() noexcept;

    /// Wrapper of `__repr__` method in Python.
    static std::string repr(const PyCases &cases) noexcept;

    /// Compare the cases contents of two PyCases.
    friend constexpr auto operator==(const PyCases &lhs, const PyCases &rhs) noexcept;

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

constexpr auto operator==(const PyCases &lhs, const PyCases &rhs) noexcept {
    return lhs.data_ref() == rhs.data_ref();
}

} // namespace klotski::ffi
