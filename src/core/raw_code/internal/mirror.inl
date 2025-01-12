#pragma once

#include "utils/common.h"

namespace klotski::codec {

///   MASK_MIRROR_H1  |   MASK_MIRROR_H2
///  111 000 000 000  |  000 111 000 000
///  111 000 000 000  |  000 111 000 000
///  111 000 000 000  |  000 111 000 000
///  111 000 000 000  |  000 111 000 000
///  111 000 000 000  |  000 111 000 000

constexpr uint64_t MASK_MIRROR_H1 = 0x0'007'007'007'007'007;
constexpr uint64_t MASK_MIRROR_H2 = 0x0'038'038'038'038'038;

///   MASK_MIRROR_V1  |   MASK_MIRROR_V2  |   MASK_MIRROR_V3
///  111 111 111 111  |  000 000 000 000  |  000 000 000 000
///  000 000 000 000  |  111 111 111 111  |  000 000 000 000
///  000 000 000 000  |  000 000 000 000  |  111 111 111 111
///  000 000 000 000  |  000 000 000 000  |  000 000 000 000
///  000 000 000 000  |  000 000 000 000  |  000 000 000 000

constexpr uint64_t MASK_MIRROR_V1 = 0x0'000'000'000'000'FFF;
constexpr uint64_t MASK_MIRROR_V2 = 0x0'000'000'000'FFF'000;
constexpr uint64_t MASK_MIRROR_V3 = 0x0'000'000'FFF'000'000;

static constexpr void vertical_fill(uint64_t &raw_code) {
    uint64_t mask = 0;
    for (int addr = 0; addr < 60; addr += 3) { // traverse every 3-bit
        switch ((raw_code >> addr) & 0b111) {
            case BLOCK_2x1:
            case BLOCK_2x2:
                mask |= (uint64_t)0b111 << (addr + 12); // generate fill mask
        }
    }
    for (int addr = 0; addr < 60; addr += 3) { // traverse every 3-bit
        switch ((raw_code | mask) >> addr & 0b111) {
            case BLOCK_2x1:
                raw_code &= ~(uint64_t(~BLOCK_2x1 & 0b111) << (addr + 12)); // fill vertical mirror
                break;
            case BLOCK_2x2:
                raw_code &= ~(uint64_t(~BLOCK_2x2 & 0b111) << (addr + 12)); // fill vertical mirror
                break;
        }
    }
}

static constexpr void horizontal_fill(uint64_t &raw_code) {
    for (int addr = 0; addr < 60; addr += 3) { // traverse every 3-bit
        switch ((raw_code >> addr) & 0b111) {
            case BLOCK_1x2:
                raw_code &= ~(uint64_t(~BLOCK_1x2 & 0b111) << (addr + 3)); // fill horizontal mirror
                addr += 3; // skip next address
                break;
            case BLOCK_2x2:
                raw_code &= ~(uint64_t(~BLOCK_2x2 & 0b111) << (addr + 3)); // fill horizontal mirror
                addr += 3; // skip next address
                break;
        }
    }
}

static constexpr void vertical_clear(uint64_t &raw_code) {
    for (int addr = 0; addr < 60; addr += 3) { // traverse every 3-bit
        switch ((raw_code >> addr) & 0b111) {
            case BLOCK_2x1:
            case BLOCK_2x2:
                raw_code |= (uint64_t)0b111 << (addr + 12); // reset as original block
        }
    }
}

static constexpr void horizontal_clear(uint64_t &raw_code) {
    for (int addr = 0; addr < 60; addr += 3) { // traverse every 3-bit
        switch ((raw_code >> addr) & 0b111) {
            case BLOCK_1x2:
            case BLOCK_2x2:
                raw_code |= (uint64_t)0b111 << (addr + 3); // reset as original block
        }
    }
}

constexpr uint64_t RawCode::get_vertical_mirror(const uint64_t raw_code) {
    uint64_t code = ((raw_code >> 36) & 0x000'000'000'fff'fff) | ((raw_code << 36) & 0xfff'fff'000'000'000);
    code = ((code >> 12) & 0x000'fff'000'000'fff) | ((code << 12) & 0xfff'000'000'fff'000);
    code |= (raw_code & 0x000'000'fff'000'000);
    uint64_t m1 = ~code & (code >> 1) & 0x249'249'249'249'249;
    uint64_t m2 = ~code & (code >> 1) & 0x492'492'492'492'492;
    m1 |= (m1 << 2);
    m2 |= (m2 >> 1);
    return (code | m1 | m2) & ~(m1 >> 12) & ~(m2 >> 12);
}

constexpr uint64_t RawCode::get_horizontal_mirror(const uint64_t raw_code) {
    uint64_t code = ((raw_code >> 6) & 0x03f'03f'03f'03f'03f) | ((raw_code << 6) & 0xfc0'fc0'fc0'fc0'fc0);
    code = ((code >> 3) & 0x1c7'1c7'1c7'1c7'1c7) | ((code << 3) & 0xe38'e38'e38'e38'e38);
    uint64_t m1 = ~code & (code << 1) & 0x492'492'492'492'492;
    uint64_t m2 = ~code & (code >> 1) & 0x492'492'492'492'492;
    m1 |= (m1 << 1);
    m2 |= (m2 >> 1);
    return (code | m1 | m2) & ~(m1 >> 3) & ~(m2 >> 3);
}

constexpr bool RawCode::check_mirror(uint64_t raw_code) {
    // horizontal_fill(raw_code);
    // return !(MASK_MIRROR_H1 & ((raw_code >> 9) ^ raw_code))
    //     && !(MASK_MIRROR_H2 & ((raw_code >> 3) ^ raw_code));

    // return raw_code == get_horizontal_mirror(raw_code);

    uint64_t kk = ~raw_code & (raw_code << 1) & 0x0492492492492492;
    kk |= (kk << 1);

    // uint64_t pp = ~raw_code & (raw_code >> 1) & 0x0492492492492492;
    // pp |= (pp >> 1);

    uint64_t pp = raw_code & 0x924924924924924;
    pp = (pp >> 1) | (pp >> 2);

    raw_code |= pp;
    raw_code &= ~(kk << 3);
    // raw_code &= ~(pp << 3);

    return !(MASK_MIRROR_H1 & ((raw_code >> 9) ^ raw_code))
        && !(MASK_MIRROR_H2 & ((raw_code >> 3) ^ raw_code));

    // return (((raw_code >> 9) & MASK_MIRROR_H1) == (raw_code & MASK_MIRROR_H1))
    //     && (((raw_code >> 3) & MASK_MIRROR_H2) == (raw_code & MASK_MIRROR_H2));

    // return ((MASK_MIRROR_H1 & ((raw_code >> 9) ^ raw_code))
    //       | (MASK_MIRROR_H2 & ((raw_code >> 3) ^ raw_code))) == 0;
}

} // namespace klotski::codec
