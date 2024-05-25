#pragma once

namespace klotski::cases {

inline Ranges& BasicRanges::get_ranges() {
    static Ranges ranges;
    return ranges;
}

inline const Ranges& BasicRanges::fetch() {
    if (!available_) {
        build();
    }
    return get_ranges();
}

inline void BasicRanges::build() {
    if (available_) {
        return; // reduce consumption of mutex
    }
    std::lock_guard guard {building_};
    if (!available_) {
        build_ranges(get_ranges());
        available_ = true;
    }
}

inline bool BasicRanges::is_available() const {
    return available_; // no mutex required in one-way state
}

} // namespace klotski::codec
