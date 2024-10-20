/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

#include <variant>

#include "py_common_code.h"
#include "ranges/ranges.h"

namespace klotski::ffi {

using cases::RangesUnion;

class PyCases {
public:
    PyCases() = delete;

    // ------------------------------------------------------------------------------------- //

    class CommonCodeIter {
    public:
        PyCommonCode next();
        explicit CommonCodeIter(const RangesUnion &data);

    private:
        uint8_t head_ {0};
        uint32_t index_ {0};
        const RangesUnion &data_;
    };

    class ShortCodeIter {
    public:
        PyShortCode next();
        explicit ShortCodeIter(CommonCodeIter iter);

    private:
        CommonCodeIter iter_;
    };

    // ------------------------------------------------------------------------------------- //

    /// Constructing from rvalue.
    static PyCases from(RangesUnion &&data) noexcept;

    /// Constructing from longer-lived reference.
    static PyCases from_ref(const RangesUnion &data) noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Get the number of cases.
    [[nodiscard]] size_t size() const;

    /// Get ShortCode iterator of cases.
    [[nodiscard]] ShortCodeIter short_codes() const;

    /// Get CommonCode iterator of cases.
    [[nodiscard]] CommonCodeIter common_codes() const;

    /// Get the CommonCode of the specified index.
    [[nodiscard]] PyCommonCode operator[](size_t index) const;

    // ------------------------------------------------------------------------------------- //

    // TODO: add len / repr

private:
    explicit PyCases(RangesUnion &&data);
    explicit PyCases(const RangesUnion &data);

    // ------------------------------------------------------------------------------------- //

    /// Get const reference of the cases data.
    [[nodiscard]] const RangesUnion& data_ref() const noexcept;

    /// Stores actual cases data or its references.
    std::variant<RangesUnion, std::reference_wrapper<const RangesUnion>> data_;

    // ------------------------------------------------------------------------------------- //
};

// TODO: allow compare

} // namespace klotski::ffi
