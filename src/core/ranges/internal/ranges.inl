#pragma once

namespace klotski::cases {

KLSK_INLINE_H void Ranges::reverse() {
    #pragma clang loop vectorize(enable)
    for (auto &x : *this) {
        x = range_reverse(x);
    }
}

inline Ranges& RangesUnion::ranges(const size_t head) {
    return std::array<Ranges, 16>::operator[](head);
}

inline const Ranges& RangesUnion::ranges(const size_t head) const {
    return std::array<Ranges, 16>::operator[](head);
}

inline Ranges& Ranges::operator+=(const Ranges &ranges) {
    this->insert(this->end(), ranges.begin(), ranges.end());
    return *this;
}

inline RangesUnion& RangesUnion::operator+=(const RangesUnion &ranges_union) {
    for (const auto head : Heads) {
        ranges(head) += ranges_union.ranges(head);
    }
    return *this;
}

KLSK_INLINE_H size_t RangesUnion::size() const {
    size_type size = 0;
    KLSK_UNROLL(sizeof(Heads))
    for (const auto head : Heads) {
        size += ranges(head).size();
    }
    return size;
}

} // namespace klotski::cases
