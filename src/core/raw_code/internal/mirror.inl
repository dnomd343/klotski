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

constexpr uint64_t RawCode::get_vertical_mirror(uint64_t raw_code) {
    // vertical_fill(raw_code);

    // uint64_t code = (raw_code & MASK_MIRROR_V3)
    //     | ((raw_code >> 48) & MASK_MIRROR_V1) | ((raw_code >> 24) & MASK_MIRROR_V2)
    //     | ((raw_code & MASK_MIRROR_V2) << 24) | ((raw_code & MASK_MIRROR_V1) << 48);

    constexpr uint64_t M_1 = 0x0'000'000'000'FFF'FFF;
    constexpr uint64_t M_2 = 0x0'FFF'FFF'000'000'000;

    constexpr uint64_t M_3 = 0x0'000'FFF'000'000'FFF;
    constexpr uint64_t M_4 = 0x0'FFF'000'000'FFF'000;

    constexpr uint64_t M_5 = 0x0'000'000'FFF'000'000;

    uint64_t code = ((raw_code >> 36) & M_1) | ((raw_code << 36) & M_2);
    code = ((code >> 12) & M_3) | ((code << 12) & M_4);
    code |= (raw_code & M_5);

    uint64_t kk = ~code & (code >> 1) & 0x0249249249249249;
    uint64_t pp = ~code & (code >> 1) & 0x0492492492492492;
    kk |= (kk << 2);
    pp |= (pp >> 1);

    // code |= (kk | pp);
    // code &= ~(kk >> 12) & ~(pp >> 12);

    return (code | kk | pp) & ~(kk >> 12) & ~(pp >> 12);

    // vertical_clear(raw_code);
    // return raw_code;
}

constexpr uint64_t RawCode::get_horizontal_mirror(uint64_t raw_code) {

    // uint64_t kk = ~raw_code & (raw_code << 1) & 0x0492492492492492;
    // kk |= (kk << 1);
    // kk = ((kk << 6) & (0x0'038'038'038'038'038 << 3)) | ((kk >> 6) & 0x0'007'007'007'007'007) | (kk & 0x0'038'038'038'038'038);

    // uint64_t pp = ~raw_code & (raw_code >> 1) & 0x0492492492492492;
    // pp |= (pp >> 1);
    // pp = ((pp << 6) & (0x0'038'038'038'038'038 << 3)) | ((pp >> 6) & 0x0'007'007'007'007'007) | (pp & 0x0'038'038'038'038'038);

    // uint64_t code = ((raw_code >> 9) & MASK_MIRROR_H1) | ((raw_code >> 3) & MASK_MIRROR_H2)
    //     | ((raw_code & MASK_MIRROR_H2) << 3) | ((raw_code & MASK_MIRROR_H1) << 9); // flip raw code

    // return code & (~kk & ~pp) | ((kk << 3) | (pp << 3));


    constexpr uint64_t MASK_1 = 0x0'007'007'007'007'007 | 0x0'038'038'038'038'038;
    constexpr uint64_t MASK_2 = MASK_1 << 6;

    constexpr uint64_t MASK_3 = 0x0'007'007'007'007'007 | (0x0'038'038'038'038'038 << 3);
    constexpr uint64_t MASK_4 = MASK_3 << 3;

    uint64_t code = ((raw_code >> 6) & MASK_1) | ((raw_code << 6) & MASK_2);
    code = ((code >> 3) & MASK_3) | ((code << 3) & MASK_4);

    uint64_t kk = ~code & (code << 1) & 0x0492492492492492;
    uint64_t pp = ~code & (code >> 1) & 0x0492492492492492;
    kk |= (kk << 1);
    pp |= (pp >> 1);

    return (code | kk | pp) & ~(kk >> 3) & ~(pp >> 3);

    // code |= (kk | pp);
    // code &= (~(kk >> 3) & ~(pp >> 3));
    // return code;
}

constexpr bool RawCode::check_mirror(uint64_t raw_code) {
    horizontal_fill(raw_code);
    return !(MASK_MIRROR_H1 & ((raw_code >> 9) ^ raw_code))
        && !(MASK_MIRROR_H2 & ((raw_code >> 3) ^ raw_code));
}

} // namespace klotski::codec
