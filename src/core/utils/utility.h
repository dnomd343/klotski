#pragma once

#include <cstdint>

#define DISALLOW_COPY_AND_ASSIGN(T) \
    T(T&&) = delete; \
    T(const T&) = delete; \
    T& operator=(T&&) = delete; \
    T& operator=(const T&) = delete;

namespace klotski {

inline int low_zero_num(uint32_t bin) {
    return __builtin_ctzl(bin);

    // WARN: be aware of serious performance issues
    // return __builtin_popcount(~(bin ^ -bin)) - 1;
}

inline uint32_t range_reverse(uint32_t bin) noexcept {
#if defined(__GNUC__) || defined(__clang__)
    bin = __builtin_bswap32(bin);
    // TODO: using `std::byteswap` (c++23)
#else
    // FIXME: `_byteswap_ulong` under MSVC
    bin = ((bin << 16) & 0xFFFF0000) | ((bin >> 16) & 0x0000FFFF);
    bin = ((bin << 8) & 0xFF00FF00) | ((bin >> 8) & 0x00FF00FF);
#endif
    bin = ((bin << 4) & 0xF0F0F0F0) | ((bin >> 4) & 0x0F0F0F0F);
    return ((bin << 2) & 0xCCCCCCCC) | ((bin >> 2) & 0x33333333);
}

} // namespace klotski
