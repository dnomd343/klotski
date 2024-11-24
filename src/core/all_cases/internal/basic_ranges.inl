#pragma once

#include "ranges/ranges.h"

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

inline bool BasicRanges::is_available() const {
    return available_; // no mutex required in one-way state
}

} // namespace klotski::codec
