#pragma once

#include "common_code/common_code.h"

namespace klotski::group {

KLSK_INLINE_CE uint_fast8_t to_type_id(const int n, const int n_2x1, const int n_1x1) {
    KLSK_ASSUME(n >= 0 && n <= 7);
    KLSK_ASSUME(n_2x1 >= 0 && n_2x1 <= n);
    KLSK_ASSUME(n_1x1 >= 0 && n_1x1 <= (14 - n * 2));
    constexpr int offset[8] = {0, 15, 41, 74, 110, 145, 175, 196};
    return offset[n] + (15 - n * 2) * n_2x1 + n_1x1;
}

KLSK_INLINE_CE uint_fast8_t GroupUnion::type_id(const codec::CommonCode common_code) {
    const auto range = static_cast<uint32_t>(common_code.unwrap());
    const auto n_1x1 = std::popcount((range >> 1) & range & 0x55555555);
    const auto n_2x1 = std::popcount((range >> 1) & ~range & 0x55555555);
    return to_type_id(std::popcount(range) - n_1x1 * 2, n_2x1, n_1x1);
}

KLSK_INLINE_CE uint_fast8_t GroupUnion::type_id(const codec::RawCode raw_code) {
    const auto code = raw_code.unwrap();
    const auto n = std::popcount(((code >> 1) ^ code) & 0x0249249249249249);
    const auto n_2x1 = std::popcount((code >> 1) & ~code & 0x0249249249249249);
    const auto n_1x1 = std::popcount((code >> 1) & code & 0x0249249249249249) - n - 3;
    return to_type_id(n, n_2x1, n_1x1);
}

} // namespace klotski::group
