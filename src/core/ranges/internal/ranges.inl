#pragma once

namespace klotski::cases {

KLSK_INLINE_H void Ranges::reverse() {
    for (auto &x : *this) {
        x = range_reverse(x);
    }
}

inline Ranges& Ranges::operator+=(const Ranges &ranges) {
    this->insert(this->end(), ranges.begin(), ranges.end());
    return *this;
}

} // namespace klotski::cases
