#pragma once

namespace klotski::cases {

KLSK_INLINE_CE int Ranges::check(const int head, uint32_t range) {
    KLSK_ASSUME(head >= 0 && head < 16 && head % 4 != 3);
    const int limit = 16 - (std::countl_zero(range) >> 1);
    uint32_t flags = 0b110011 << head; // fill 2x2 block

    for (int offset = 0, addr = 0; offset < limit; ++offset, range >>= 2) { // traverse every 2-bit
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

} // namespace klotski::cases
