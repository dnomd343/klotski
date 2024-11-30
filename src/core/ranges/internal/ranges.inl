#pragma once

#include <span>

#include "utils/utility.h"
#include "common_code/common_code.h"

namespace klotski::cases {

KLSK_INLINE_H void Ranges::reverse() {
    KLSK_IVDEP
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

KLSK_INLINE_H RangesUnion& RangesUnion::operator+=(const RangesUnion &ranges_union) {
    KLSK_UNROLL(Heads.size())
    for (const auto head : Heads) {
        ranges(head) += ranges_union.ranges(head);
    }
    return *this;
}

KLSK_INLINE_H size_t RangesUnion::size() const {
    size_type size = 0;
    KLSK_UNROLL(Heads.size())
    for (const auto head : Heads) {
        size += ranges(head).size();
    }
    return size;
}

KLSK_INLINE_H codec::CommonCode RangesUnion::operator[](size_type n) const {
    if (n < ranges(0).size()) {
        return codec::CommonCode::unsafe_create(ranges(0)[n]);
    }
    n -= ranges(0).size();

    KLSK_UNROLL(Heads.size() - 2)
    for (const uint64_t head : std::span {Heads.data() + 1, Heads.size() - 2}) {
        if (n < ranges(head).size()) {
            return codec::CommonCode::unsafe_create(head << 32 | ranges(head)[n]);
        }
        n -= ranges(head).size();
    }

    constexpr auto head = static_cast<uint64_t>(0xE);
    return codec::CommonCode::unsafe_create(head << 32 | ranges(0xE)[n]);
}

} // namespace klotski::cases
