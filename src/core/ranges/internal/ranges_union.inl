#pragma once

namespace klotski::cases {

inline Ranges& RangesUnion::ranges(const size_t head) {
    return std::array<Ranges, 16>::operator[](head);
}

inline const Ranges& RangesUnion::ranges(const size_t head) const {
    return std::array<Ranges, 16>::operator[](head);
}

KLSK_INLINE_H size_t RangesUnion::size() const {
    size_type size = 0;
    KLSK_UNROLL(sizeof(Heads))
    for (const auto head : Heads) {
        size += ranges(head).size();
    }
    return size;
}

// KLSK_INLINE_H uint32_t RangesUnion::operator[](size_type n) const {
//     size_t head = 0;
//     for (;;) {
//         if (n >= ranges(head).size()) {
//             n -= ranges(head).size();
//             ++head;
//         } else {
//             break;
//         }
//     }
//     return ranges(head)[n];
// }

} // namespace klotski::cases
