#include "raw_code.h"
#include "common.h"

using klotski::RawCode;

/// ----------------------------- Mirror Convert ------------------------------

RawCode RawCode::to_vertical_mirror() const noexcept {
    return RawCode::unsafe_create(get_vertical_mirror(code_));
}

RawCode RawCode::to_horizontal_mirror() const noexcept {
    return RawCode::unsafe_create(get_horizontal_mirror(code_));
}

/// ------------------------------ Mirror Check -------------------------------

bool RawCode::is_vertical_mirror() const noexcept {
    return check_vertical_mirror(code_);
}

bool RawCode::is_horizontal_mirror() const noexcept {
    return check_horizontal_mirror(code_);
}

bool RawCode::is_vertical_mirror(RawCode &&raw_code) const noexcept {
    return raw_code.unwrap() == get_vertical_mirror(code_);
}

bool RawCode::is_horizontal_mirror(RawCode &&raw_code) const noexcept {
    return raw_code.unwrap() == get_horizontal_mirror(code_);
}

bool RawCode::is_vertical_mirror(const RawCode &raw_code) const noexcept {
    return raw_code.unwrap() == get_vertical_mirror(code_);
}

bool RawCode::is_horizontal_mirror(const RawCode &raw_code) const noexcept {
    return raw_code.unwrap() == get_horizontal_mirror(code_);
}

/// ----------------------------- Basic Functions -----------------------------

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

inline void vertical_fill(uint64_t &raw_code) {
    uint64_t mask = 0;
    for (int addr = 0; addr < 60; addr += 3) { // traverse every 3-bit
        switch ((raw_code >> addr) & 0b111) {
            case B_2x1:
            case B_2x2:
                mask |= (uint64_t)0b111 << (addr + 12); // generate fill mask
        }
    }
    for (int addr = 0; addr < 60; addr += 3) { // traverse every 3-bit
        switch ((raw_code | mask) >> addr & 0b111) {
            case B_2x1:
                raw_code &= ~(uint64_t(~B_2x1 & 0b111) << (addr + 12)); // fill vertical mirror
                break;
            case B_2x2:
                raw_code &= ~(uint64_t(~B_2x2 & 0b111) << (addr + 12)); // fill vertical mirror
                break;
        }
    }
}

inline void horizontal_fill(uint64_t &raw_code) {
    for (int addr = 0; addr < 60; addr += 3) { // traverse every 3-bit
        switch ((raw_code >> addr) & 0b111) {
            case B_1x2:
                raw_code &= ~(uint64_t(~B_1x2 & 0b111) << (addr + 3)); // fill horizontal mirror
                addr += 3; // skip next address
                break;
            case B_2x2:
                raw_code &= ~(uint64_t(~B_2x2 & 0b111) << (addr + 3)); // fill horizontal mirror
                addr += 3; // skip next address
                break;
        }
    }
}

inline void vertical_clear(uint64_t &raw_code) {
    for (int addr = 0; addr < 60; addr += 3) { // traverse every 3-bit
        switch ((raw_code >> addr) & 0b111) {
            case B_2x1:
            case B_2x2:
                raw_code |= (uint64_t)0b111 << (addr + 12); // reset as original block
        }
    }
}

inline void horizontal_clear(uint64_t &raw_code) {
    for (int addr = 0; addr < 60; addr += 3) { // traverse every 3-bit
        switch ((raw_code >> addr) & 0b111) {
            case B_1x2:
            case B_2x2:
                raw_code |= (uint64_t)0b111 << (addr + 3); // reset as original block
        }
    }
}

uint64_t RawCode::get_vertical_mirror(uint64_t raw_code) noexcept {
    vertical_fill(raw_code);
    raw_code = (raw_code & MASK_MIRROR_V3)
        | ((raw_code >> 48) & MASK_MIRROR_V1) | ((raw_code >> 24) & MASK_MIRROR_V2)
        | ((raw_code & MASK_MIRROR_V2) << 24) | ((raw_code & MASK_MIRROR_V1) << 48);
    vertical_clear(raw_code);
    return raw_code;
}

uint64_t RawCode::get_horizontal_mirror(uint64_t raw_code) noexcept {
    horizontal_fill(raw_code);
    raw_code = ((raw_code >> 9) & MASK_MIRROR_H1) | ((raw_code >> 3) & MASK_MIRROR_H2)
        | ((raw_code & MASK_MIRROR_H2) << 3) | ((raw_code & MASK_MIRROR_H1) << 9); // flip raw code
    horizontal_clear(raw_code);
    return raw_code;
}

bool RawCode::check_vertical_mirror(uint64_t raw_code) noexcept {
    vertical_fill(raw_code);
    return !(MASK_MIRROR_V1 & ((raw_code >> 48) ^ raw_code))
        && !(MASK_MIRROR_V2 & ((raw_code >> 24) ^ raw_code));
}

bool RawCode::check_horizontal_mirror(uint64_t raw_code) noexcept {
    horizontal_fill(raw_code);
    return !(MASK_MIRROR_H1 & ((raw_code >> 9) ^ raw_code))
        && !(MASK_MIRROR_H2 & ((raw_code >> 3) ^ raw_code));
}
