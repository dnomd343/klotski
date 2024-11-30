#pragma once

namespace klotski::cases {

template <int N>
KLSK_INLINE_CE int check_range(uint32_t flags, uint32_t range) {
    for (int offset = 0, addr = 0; offset < N; ++offset, range >>= 2) { // traverse every 2-bit
        addr += std::countr_one(flags); // next unfilled block
        flags >>= std::countr_one(flags);
        switch (range & 0b11) {
        case 0b00: // space
        case 0b11: // 1x1 block
            flags |= 0b1;
            continue;
        case 0b01: // 1x2 block
            if (flags & 0b10 || addr % 4 == 3) { // invalid case
                return offset + 1; // broken offset
            }
            flags |= 0b11;
            continue;
        case 0b10: // 2x1 block
            if (flags & 0b10000 || addr > 15) { // invalid case
                return offset + 1; // broken offset
            }
            flags |= 0b10001;
            continue;
        default:
            std::unreachable();
        }
    }
    return 0; // pass check
}

KLSK_INLINE_CE int Ranges::check(const int head, const uint32_t range) {
    KLSK_ASSUME(head >= 0 && head < 16 && head % 4 != 3);
    const uint32_t flags = 0b110011 << head; // fill 2x2 block
    switch (std::countl_zero(range) >> 1) {
        case 0: return check_range<16>(flags, range);
        case 1: return check_range<15>(flags, range);
        case 2: return check_range<14>(flags, range);
        case 3: return check_range<13>(flags, range);
        case 4: return check_range<12>(flags, range);
        case 5: return check_range<11>(flags, range);
        case 6: return check_range<10>(flags, range);
        case 7: return check_range<9>(flags, range);
        case 8: return check_range<8>(flags, range);
        case 9: return check_range<7>(flags, range);
        case 10: return check_range<6>(flags, range);
        case 11: return check_range<5>(flags, range);
        case 12: return check_range<4>(flags, range);
        case 13: return check_range<3>(flags, range);
        case 14: return check_range<2>(flags, range);
        case 15: return check_range<1>(flags, range);
        case 16: return check_range<0>(flags, range);
        default: std::unreachable();
    }
}

} // namespace klotski::cases
