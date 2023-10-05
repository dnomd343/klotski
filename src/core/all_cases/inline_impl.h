#pragma once

namespace klotski {
namespace cases {

// ----------------------------------------------------------------------------------------- //

inline BasicRanges& BasicRanges::instance() noexcept {
    static BasicRanges instance;
    return instance;
}

inline Ranges& BasicRanges::get_ranges() noexcept {
    static Ranges ranges;
    return ranges;
}

inline const Ranges& BasicRanges::fetch() noexcept {
    this->build();
    return get_ranges();
}

inline bool BasicRanges::is_available() const noexcept {
    return available_; // no mutex required in one-way state
}

// ----------------------------------------------------------------------------------------- //

inline AllCases& AllCases::instance() noexcept {
    static AllCases instance;
    return instance;
}

inline RangesUnion& AllCases::get_cases() noexcept {
    static RangesUnion cases;
    return cases;
}

inline const RangesUnion& AllCases::fetch() noexcept {
    this->build();
    return get_cases();
}

inline bool AllCases::is_available() const noexcept {
    return available_; // no mutex required in one-way state
}

// ----------------------------------------------------------------------------------------- //

} // namespace codec
} // namespace klotski
