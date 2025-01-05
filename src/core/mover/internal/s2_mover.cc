#include <iostream>

#include "mover/s2_mover.h"
#include "raw_code/raw_code.h"

using klotski::mover::S2Mover;
using klotski::codec::RawCode;

#define CAPTURE(code, addr) \
    (((code) >> ((addr) * 3)) & (uint64_t)(0b111))

#define UNSET_1x1_(code, addr) \
    ((code) & ~((uint64_t)0b111 << ((addr) * 3)))

#define SET_1x1_(code, addr) \
    ((code) | ((uint64_t)0b011 << ((addr) * 3)))

#define MOVE_1x1(code, addr_old, addr_new) \
    SET_1x1_(UNSET_1x1_(code, addr_old), addr_new)

#define UNSET_1x2_(code, addr) \
    ((code) & ~((uint64_t)0b111'111 << ((addr) * 3)))

#define SET_1x2_(code, addr) \
    ((code) | ((uint64_t)0b111'001 << ((addr) * 3)))

#define MOVE_1x2(code, addr_old, addr_new) \
    SET_1x2_(UNSET_1x2_(code, addr_old), addr_new)

#define UNSET_2x1_(code, addr) \
    ((code) & ~((uint64_t)0b111'000'000'000'111 << ((addr) * 3)))

#define SET_2x1_(code, addr) \
    ((code) | ((uint64_t)0b111'000'000'000'010 << ((addr) * 3)))

#define MOVE_2x1(code, addr_old, addr_new) \
    SET_2x1_(UNSET_2x1_(code, addr_old), addr_new)

#define UNSET_2x2_(code, addr) \
    ((code) & ~((uint64_t)0b111'111'000'000'111'111 << ((addr) * 3)))

#define SET_2x2_(code, addr) \
    ((code) | ((uint64_t)0b111'111'000'000'111'100 << ((addr) * 3)))

#define MOVE_2x2(code, addr_old, addr_new) \
    SET_2x2_(UNSET_2x2_(code, addr_old), addr_new)

template <int N>
void S2Mover::move_single(const uint64_t code) const {
    if (N >= 4) { // case up
        if (CAPTURE(code, N - 4) == BLOCK_1x1) {
            release_(MOVE_1x1(code, N - 4, N));
        } else if (N >= 8 && CAPTURE(code, N - 8) == BLOCK_2x1) {
            release_(MOVE_2x1(code, N - 8, N - 4));
        }
    }

    if (N < 16) { // case down
        if (const uint8_t block = CAPTURE(code, N + 4); block == BLOCK_1x1) {
            release_(MOVE_1x1(code, N + 4, N));
        } else if (N < 12 && block == BLOCK_2x1) {
            release_(MOVE_2x1(code, N + 4, N));
        }
    }

    if (N % 4 >= 1) { // case left
        if (CAPTURE(code, N - 1) == BLOCK_1x1) {
            release_(MOVE_1x1(code, N - 1, N));
        } else if (N % 4 >= 2 && CAPTURE(code, N - 2) == BLOCK_1x2) {
            release_(MOVE_1x2(code, N - 2, N - 1));
        }
    }

    if (N % 4 < 3) { // case right
        if (const uint8_t block = CAPTURE(code, N + 1); block == BLOCK_1x1) {
            release_(MOVE_1x1(code, N + 1, N));
        } else if (N % 4 < 2 && block == BLOCK_1x2) {
            release_(MOVE_1x2(code, N + 1, N));
        }
    }
}

template <int N>
void S2Mover::move_double_h(const uint64_t code) const {
    do { // case up
        if (N >= 4) {
            if (const uint8_t block = CAPTURE(code, N - 4); block == BLOCK_1x1) { // left part
                release_(MOVE_1x1(code, N - 4, N));
                release_(MOVE_1x1(code, N - 4, N + 1));
            } else if (block == BLOCK_1x2) {
                release_(MOVE_1x2(code, N - 4, N));
                break;
            } else if (N >= 8 && block == BLOCK_fill) {
                if (const uint8_t block_ = CAPTURE(code, N - 8); block_ == BLOCK_2x2) {
                    release_(MOVE_2x2(code, N - 8, N - 4));
                    break;
                } else if (block_ == BLOCK_2x1) {
                    release_(MOVE_2x1(code, N - 8, N - 4));
                }
            }
            if (const uint8_t block = CAPTURE(code, N - 3); block == BLOCK_1x1) { // right part
                release_(MOVE_1x1(code, N - 3, N + 1));
                release_(MOVE_1x1(code, N - 3, N));
            } else if (N >= 8 && block == BLOCK_fill && CAPTURE(code, N - 7) == BLOCK_2x1) {
                release_(MOVE_2x1(code, N - 7, N - 3));
            }
        }
    } while (false);

    do { // case down
        if (N < 16) {
            if (const uint8_t block = CAPTURE(code, N + 4); block == BLOCK_1x1) { // left part
                release_(MOVE_1x1(code, N + 4, N));
                release_(MOVE_1x1(code, N + 4, N + 1));
            } else if (N < 12 && block == BLOCK_2x1) {
                release_(MOVE_2x1(code, N + 4, N));
            } else if (block == BLOCK_1x2) {
                release_(MOVE_1x2(code, N + 4, N));
                break;
            } else if (N < 12 && block == BLOCK_2x2) {
                release_(MOVE_2x2(code, N + 4, N));
                break;
            }
            if (const uint8_t block = CAPTURE(code, N + 5); block == BLOCK_1x1) { // right part
                release_(MOVE_1x1(code, N + 5, N + 1));
                release_(MOVE_1x1(code, N + 5, N));
            } else if (N < 12 && block == BLOCK_2x1) {
                release_(MOVE_2x1(code, N + 5, N + 1));
            }
        }
    } while (false);

    if (N % 4 >= 1) { // case left
        if (CAPTURE(code, N - 1) == BLOCK_1x1) {
            release_(MOVE_1x1(code, N - 1, N));
            release_(MOVE_1x1(code, N - 1, N + 1));
        } else if (N % 4 == 2 && CAPTURE(code, N - 2) == BLOCK_1x2) {
            release_(MOVE_1x2(code, N - 2, N - 1));
            release_(MOVE_1x2(code, N - 2, N));
        }
    }

    if (N % 4 < 2) { // case right
        if (const uint8_t block = CAPTURE(code, N + 2); block == BLOCK_1x1) {
            release_(MOVE_1x1(code, N + 2, N + 1));
            release_(MOVE_1x1(code, N + 2, N));
        } else if (N % 4 == 0 && block == BLOCK_1x2) {
            release_(MOVE_1x2(code, N + 2, N + 1));
            release_(MOVE_1x2(code, N + 2, N));
        }
    }
}

template <int N>
void S2Mover::move_double_v(const uint64_t code) const {
    if (N >= 4) { // case up
        if (CAPTURE(code, N - 4) == BLOCK_1x1) {
            release_(MOVE_1x1(code, N - 4, N));
            release_(MOVE_1x1(code, N - 4, N + 4));
        } else if (N >= 8 && CAPTURE(code, N - 8) == BLOCK_2x1) {
            release_(MOVE_2x1(code, N - 8, N - 4));
            release_(MOVE_2x1(code, N - 8, N));
        }
    }

    if (N < 12) { // case down
        if (CAPTURE(code, N + 8) == BLOCK_1x1) {
            release_(MOVE_1x1(code, N + 8, N + 4));
            release_(MOVE_1x1(code, N + 8, N));
        } else if (N < 8 && CAPTURE(code, N + 8) == BLOCK_2x1) {
            release_(MOVE_2x1(code, N + 8, N + 4));
            release_(MOVE_2x1(code, N + 8, N));
        }
    }

    do { // case left
        if (N % 4 != 0) {
            if (const uint8_t block = CAPTURE(code, N - 1); block == BLOCK_1x1) { // up part
                release_(MOVE_1x1(code, N - 1, N));
                release_(MOVE_1x1(code, N - 1, N + 4));
            } else if (block == BLOCK_2x1) {
                release_(MOVE_2x1(code, N - 1, N));
                break;
            } else if (N % 4 >= 2 && block == BLOCK_fill) {
                if (const uint8_t block_ = CAPTURE(code, N - 2); block_ == BLOCK_2x2) {
                    release_(MOVE_2x2(code, N - 2, N - 1));
                    break;
                } else if (block_ == BLOCK_1x2) {
                    release_(MOVE_1x2(code, N - 2, N - 1));
                }
            }
            if (const uint8_t block = CAPTURE(code, N + 3); block == BLOCK_1x1) { // down part
                release_(MOVE_1x1(code, N + 3, N + 4));
                release_(MOVE_1x1(code, N + 3, N));
            } else if (N % 4 >= 2 && block == BLOCK_fill && CAPTURE(code, N + 2) == BLOCK_1x2) {
                release_(MOVE_1x2(code, N + 2, N + 3));
            }
        }
    } while (false);

    do { // case right
        if (N % 4 < 3) {
            if (const uint8_t block = CAPTURE(code, N + 1); block == BLOCK_1x1) { // up part
                release_(MOVE_1x1(code, N + 1, N));
                release_(MOVE_1x1(code, N + 1, N + 4));
            } else if (N % 4 < 2 && block == BLOCK_1x2) {
                release_(MOVE_1x2(code, N + 1, N));
            } else if (block == BLOCK_2x1) {
                release_(MOVE_2x1(code, N + 1, N));
                break;
            } else if (N % 4 < 2 && block == BLOCK_2x2) {
                release_(MOVE_2x2(code, N + 1, N));
                break;
            }
            if (const uint8_t block = CAPTURE(code, N + 5); block == BLOCK_1x1) { // down part
                release_(MOVE_1x1(code, N + 5, N + 4));
                release_(MOVE_1x1(code, N + 5, N));
            } else if (N % 4 < 2 && block == BLOCK_1x2) {
                release_(MOVE_1x2(code, N + 5, N + 4));
            }
        }
    } while (false);
}

void S2Mover::move_double_h(uint64_t code, int offset) const {
    switch (offset) {
    case 0: move_double_h<0>(code); break;
    case 1: move_double_h<1>(code); break;
    case 2: move_double_h<2>(code); break;
    case 4: move_double_h<4>(code); break;
    case 5: move_double_h<5>(code); break;
    case 6: move_double_h<6>(code); break;
    case 8: move_double_h<8>(code); break;
    case 9: move_double_h<9>(code); break;
    case 10: move_double_h<10>(code); break;
    case 12: move_double_h<12>(code); break;
    case 13: move_double_h<13>(code); break;
    case 14: move_double_h<14>(code); break;
    case 16: move_double_h<16>(code); break;
    case 17: move_double_h<17>(code); break;
    case 18: move_double_h<18>(code); break;
    default: std::unreachable();
    }
}

void S2Mover::move_double_v(uint64_t code, int offset) const {
    switch (offset) {
    case 0: move_double_v<0>(code); break;
    case 1: move_double_v<1>(code); break;
    case 2: move_double_v<2>(code); break;
    case 3: move_double_v<3>(code); break;
    case 4: move_double_v<4>(code); break;
    case 5: move_double_v<5>(code); break;
    case 6: move_double_v<6>(code); break;
    case 7: move_double_v<7>(code); break;
    case 8: move_double_v<8>(code); break;
    case 9: move_double_v<9>(code); break;
    case 10: move_double_v<10>(code); break;
    case 11: move_double_v<11>(code); break;
    case 12: move_double_v<12>(code); break;
    case 13: move_double_v<13>(code); break;
    case 14: move_double_v<14>(code); break;
    case 15: move_double_v<15>(code); break;
    default: std::unreachable();
    }
}

void S2Mover::move_single(uint64_t code, int offset) const {
    switch (offset) {
    case 0: move_single<0>(code); break;
    case 1: move_single<1>(code); break;
    case 2: move_single<2>(code); break;
    case 3: move_single<3>(code); break;
    case 4: move_single<4>(code); break;
    case 5: move_single<5>(code); break;
    case 6: move_single<6>(code); break;
    case 7: move_single<7>(code); break;
    case 8: move_single<8>(code); break;
    case 9: move_single<9>(code); break;
    case 10: move_single<10>(code); break;
    case 11: move_single<11>(code); break;
    case 12: move_single<12>(code); break;
    case 13: move_single<13>(code); break;
    case 14: move_single<14>(code); break;
    case 15: move_single<15>(code); break;
    case 16: move_single<16>(code); break;
    case 17: move_single<17>(code); break;
    case 18: move_single<18>(code); break;
    case 19: move_single<19>(code); break;
    default: std::unreachable();
    }
}

void S2Mover::next_cases(const uint64_t code) {

    uint64_t tmp = (code | (code >> 1) | (code >> 2)) | ~0x0249249249249249;

    // constexpr auto kk = std::to_array({
    //     0, 0, 0, 1, 0, 0, 2, 0, 0, 3, 0, 0,
    //     4, 0, 0, 5, 0, 0, 6, 0, 0, 7, 0, 0,
    //     8, 0, 0, 9, 0, 0, 10, 0, 0, 11, 0, 0,
    //     12, 0, 0, 13, 0, 0, 14, 0, 0, 15, 0, 0,
    //     16, 0, 0, 17, 0, 0, 18, 0, 0, 19, 0, 0,
    // });

    // int val_1 = std::countr_one(tmp);
    // int val_2 = 63 - std::countl_one(tmp);

    // KLSK_ASSUME(val_1 >= 0 && val_1 <= 57 && val_1 % 3 == 0);
    // KLSK_ASSUME(val_2 >= 0 && val_2 <= 57 && val_2 % 3 == 0);

    // int space_1 = kk[val_1];
    // int space_2 = kk[val_2];
    // int space_1 = val_1 / 3;
    // int space_2 = val_2 / 3;

    int space_1 = std::countr_one(tmp) / 3;
    int space_2 = (63 - std::countl_one(tmp)) / 3;

    if (space_1 + 1 == space_2 && space_1 % 4 != 3) {
        move_double_h(code, space_1);
    } else if (space_1 + 4 == space_2) {
        move_double_v(code, space_1);
    } else {
        move_single(code, space_1);
        move_single(code, space_2);
    }
}
