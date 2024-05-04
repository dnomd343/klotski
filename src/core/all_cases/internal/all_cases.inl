#pragma once

namespace klotski::cases {

inline RangesUnion& AllCases::get_cases() {
    static RangesUnion cases;
    return cases;
}

inline const RangesUnion& AllCases::fetch() {
    if (!available_) {
        build();
    }
    return get_cases();
}

inline bool AllCases::is_available() const {
    return available_; // no mutex required in one-way state
}

} // namespace klotski::codec
