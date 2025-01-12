#include "mover/s2_hint_mover.h"

#include <iostream>

#include "raw_code/raw_code.h"

using klotski::codec::RawCode;
using klotski::mover::S2HintMoverPro;

#define ADDR(N) ((N) * 3)

#define CAPTURE(code, N) (((code) >> ADDR(N)) & 0b111ULL)

#define MOVE_1x1(code, SRC, DST) \
    ((code) & ~(K_MASK_1x1_ << ADDR(SRC)) | (K_MASK_1x1 << ADDR(DST)))

#define MOVE_1x2(code, SRC, DST) \
    ((code) & ~(K_MASK_1x2_ << ADDR(SRC)) | (K_MASK_1x2 << ADDR(DST)))

#define MOVE_2x1(code, SRC, DST) \
    ((code) & ~(K_MASK_2x1_ << ADDR(SRC)) | (K_MASK_2x1 << ADDR(DST)))

#define MOVE_2x2(code, SRC, DST) \
    ((code) & ~(K_MASK_2x2_ << ADDR(SRC)) | (K_MASK_2x2 << ADDR(DST)))

#define RELEASE_1x1(code, SRC, DST, SP) \
    release_(MOVE_1x1(code, SRC, DST), ((uint64_t)0b1 << (DST + 32)) | (SP))

#define RELEASE_1x2(code, SRC, DST, SP) \
    release_(MOVE_1x2(code, SRC, DST), ((uint64_t)0b11 << (DST + 32)) | (SP))

#define RELEASE_2x1(code, SRC, DST, SP) \
    release_(MOVE_2x1(code, SRC, DST), ((uint64_t)0b10001 << (DST + 32)) | (SP))

#define RELEASE_2x2(code, SRC, DST, SP) \
    release_(MOVE_2x2(code, SRC, DST), ((uint64_t)0b110011 << (DST + 32)) | (SP))

#define MAYBE_UP(hint, N) (((hint) & (1 << ((N) - 4))) == 0)

#define MAYBE_DOWN(hint, N) (((hint) & (1 << ((N) + 4))) == 0)

#define MAYBE_LEFT(hint, N) (((hint) & (1 << ((N) - 1))) == 0)

#define MAYBE_RIGHT(hint, N) (((hint) & (1 << ((N) + 1))) == 0)

#define MAKE_SP(N) ((uint64_t)1 << (N))

template <int N>
void S2HintMoverPro::move_single(const uint64_t code, const uint32_t hint, const uint32_t kk) const {
    if (N >= 4 && MAYBE_UP(hint, N)) { // case up
        if (CAPTURE(code, N - 4) == BLOCK_1x1) {
            RELEASE_1x1(code, N - 4, N, MAKE_SP(N - 4) | kk);
        } else if (N >= 8 && CAPTURE(code, N - 8) == BLOCK_2x1) {
            RELEASE_2x1(code, N - 8, N - 4, MAKE_SP(N - 8) | kk);
        }
    }

    if (N < 16 && MAYBE_DOWN(hint, N)) { // case down
        if (const uint8_t block = CAPTURE(code, N + 4); block == BLOCK_1x1) {
            RELEASE_1x1(code, N + 4, N, MAKE_SP(N + 4) | kk);
        } else if (N < 12 && block == BLOCK_2x1) {
            RELEASE_2x1(code, N + 4, N, MAKE_SP(N + 8) | kk);
        }
    }

    if (N % 4 >= 1 && MAYBE_LEFT(hint, N)) { // case left
        if (CAPTURE(code, N - 1) == BLOCK_1x1) {
            RELEASE_1x1(code, N - 1, N, MAKE_SP(N - 1) | kk);
        } else if (N % 4 >= 2 && CAPTURE(code, N - 2) == BLOCK_1x2) {
            RELEASE_1x2(code, N - 2, N - 1, MAKE_SP(N - 2) | kk);
        }
    }

    if (N % 4 < 3 && MAYBE_RIGHT(hint, N)) { // case right
        if (const uint8_t block = CAPTURE(code, N + 1); block == BLOCK_1x1) {
            RELEASE_1x1(code, N + 1, N, MAKE_SP(N + 1) | kk);
        } else if (N % 4 < 2 && block == BLOCK_1x2) {
            RELEASE_1x2(code, N + 1, N, MAKE_SP(N + 2) | kk);
        }
    }
}

template <int N>
void S2HintMoverPro::move_double_h(const uint64_t code, const uint32_t hint) const {
    do { // case up
        if (N >= 4 && MAYBE_UP(hint, N)) {
            if (const uint8_t block = CAPTURE(code, N - 4); block == BLOCK_1x1) { // left part
                RELEASE_1x1(code, N - 4, N, MAKE_SP(N - 4) | MAKE_SP(N + 1));
                RELEASE_1x1(code, N - 4, N + 1, MAKE_SP(N - 4) | MAKE_SP(N));
            } else if (block == BLOCK_1x2) {
                RELEASE_1x2(code, N - 4, N, MAKE_SP(N - 4) | MAKE_SP(N - 3));
                break;
            } else if (N >= 8 && block == BLOCK_fill) {
                if (const uint8_t block_ = CAPTURE(code, N - 8); block_ == BLOCK_2x2) {
                    RELEASE_2x2(code, N - 8, N - 4, MAKE_SP(N - 8) | MAKE_SP(N - 7));
                    break;
                } else if (block_ == BLOCK_2x1) {
                    RELEASE_2x1(code, N - 8, N - 4, MAKE_SP(N - 8) | MAKE_SP(N + 1));
                }
            }
            if (!MAYBE_UP(hint, N + 1)) {
                break;
            }
            if (const uint8_t block = CAPTURE(code, N - 3); block == BLOCK_1x1) { // right part
                RELEASE_1x1(code, N - 3, N + 1, MAKE_SP(N) | MAKE_SP(N - 3));
                RELEASE_1x1(code, N - 3, N, MAKE_SP(N - 3) | MAKE_SP(N + 1));
            } else if (N >= 8 && block == BLOCK_fill && CAPTURE(code, N - 7) == BLOCK_2x1) {
                RELEASE_2x1(code, N - 7, N - 3, MAKE_SP(N) | MAKE_SP(N - 7));
            }
        }
    } while (false);

    do { // case down
        if (N < 16 && MAYBE_DOWN(hint, N)) {
            if (const uint8_t block = CAPTURE(code, N + 4); block == BLOCK_1x1) { // left part
                RELEASE_1x1(code, N + 4, N, MAKE_SP(N + 4) | MAKE_SP(N + 1));
                RELEASE_1x1(code, N + 4, N + 1, MAKE_SP(N + 4) | MAKE_SP(N));
            } else if (N < 12 && block == BLOCK_2x1) {
                RELEASE_2x1(code, N + 4, N, MAKE_SP(N + 8) | MAKE_SP(N + 1));
            } else if (block == BLOCK_1x2) {
                RELEASE_1x2(code, N + 4, N, MAKE_SP(N + 4) | MAKE_SP(N + 5));
                break;
            } else if (N < 12 && block == BLOCK_2x2) {
                RELEASE_2x2(code, N + 4, N, MAKE_SP(N + 8) | MAKE_SP(N + 9));
                break;
            }
            if (!MAYBE_DOWN(hint, N + 1)) {
                break;
            }
            if (const uint8_t block = CAPTURE(code, N + 5); block == BLOCK_1x1) { // right part
                RELEASE_1x1(code, N + 5, N + 1, MAKE_SP(N) | MAKE_SP(N + 5));
                RELEASE_1x1(code, N + 5, N, MAKE_SP(N + 1) | MAKE_SP(N + 5));
            } else if (N < 12 && block == BLOCK_2x1) {
                RELEASE_2x1(code, N + 5, N + 1, MAKE_SP(N) | MAKE_SP(N + 9));
            }
        }
    } while (false);

    if (N % 4 >= 1 && MAYBE_LEFT(hint, N)) { // case left
        if (CAPTURE(code, N - 1) == BLOCK_1x1) {
            RELEASE_1x1(code, N - 1, N, MAKE_SP(N - 1) | MAKE_SP(N + 1));
            RELEASE_1x1(code, N - 1, N + 1, MAKE_SP(N - 1) | MAKE_SP(N));
        } else if (N % 4 == 2 && CAPTURE(code, N - 2) == BLOCK_1x2) {
            RELEASE_1x2(code, N - 2, N - 1, MAKE_SP(N - 2) | MAKE_SP(N + 1));
            RELEASE_1x2(code, N - 2, N, MAKE_SP(N - 2) | MAKE_SP(N - 1));
        }
    }

    if (N % 4 < 2 && MAYBE_RIGHT(hint, N + 1)) { // case right
        if (const uint8_t block = CAPTURE(code, N + 2); block == BLOCK_1x1) {
            RELEASE_1x1(code, N + 2, N + 1, MAKE_SP(N) | MAKE_SP(N + 2));
            RELEASE_1x1(code, N + 2, N, MAKE_SP(N + 1) | MAKE_SP(N + 2));
        } else if (N % 4 == 0 && block == BLOCK_1x2) {
            RELEASE_1x2(code, N + 2, N + 1, MAKE_SP(N) | MAKE_SP(N + 3));
            RELEASE_1x2(code, N + 2, N, MAKE_SP(N + 2) | MAKE_SP(N + 3));
        }
    }
}

template <int N>
void S2HintMoverPro::move_double_v(const uint64_t code, const uint32_t hint) const {
    if (N >= 4 && MAYBE_UP(hint, N)) { // case up
        if (CAPTURE(code, N - 4) == BLOCK_1x1) {
            RELEASE_1x1(code, N - 4, N, MAKE_SP(N - 4) | MAKE_SP(N + 4));
            RELEASE_1x1(code, N - 4, N + 4, MAKE_SP(N - 4) | MAKE_SP(N));
        } else if (N >= 8 && CAPTURE(code, N - 8) == BLOCK_2x1) {
            RELEASE_2x1(code, N - 8, N - 4, MAKE_SP(N - 8) | MAKE_SP(N + 4));
            RELEASE_2x1(code, N - 8, N, MAKE_SP(N - 8) | MAKE_SP(N - 4));
        }
    }

    if (N < 12 && MAYBE_DOWN(hint, N + 4)) { // case down
        if (CAPTURE(code, N + 8) == BLOCK_1x1) {
            RELEASE_1x1(code, N + 8, N + 4, MAKE_SP(N) | MAKE_SP(N + 8));
            RELEASE_1x1(code, N + 8, N, MAKE_SP(N + 4) | MAKE_SP(N + 8));
        } else if (N < 8 && CAPTURE(code, N + 8) == BLOCK_2x1) {
            RELEASE_2x1(code, N + 8, N + 4, MAKE_SP(N) | MAKE_SP(N + 12));
            RELEASE_2x1(code, N + 8, N, MAKE_SP(N + 8) | MAKE_SP(N + 12));
        }
    }

    do { // case left
        if (N % 4 != 0 && MAYBE_LEFT(hint, N)) {
            if (const uint8_t block = CAPTURE(code, N - 1); block == BLOCK_1x1) { // up part
                RELEASE_1x1(code, N - 1, N, MAKE_SP(N - 1) | MAKE_SP(N + 4));
                RELEASE_1x1(code, N - 1, N + 4, MAKE_SP(N - 1) | MAKE_SP(N));
            } else if (block == BLOCK_2x1) {
                RELEASE_2x1(code, N - 1, N, MAKE_SP(N - 1) | MAKE_SP(N + 3));
                break;
            } else if (N % 4 >= 2 && block == BLOCK_fill) {
                if (const uint8_t block_ = CAPTURE(code, N - 2); block_ == BLOCK_2x2) {
                    RELEASE_2x2(code, N - 2, N - 1, MAKE_SP(N - 2) | MAKE_SP(N + 2));
                    break;
                } else if (block_ == BLOCK_1x2) {
                    RELEASE_1x2(code, N - 2, N - 1, MAKE_SP(N - 2) | MAKE_SP(N + 4));
                }
            }
            if (!MAYBE_LEFT(hint, N + 4)) {
                break;
            }
            if (const uint8_t block = CAPTURE(code, N + 3); block == BLOCK_1x1) { // down part
                RELEASE_1x1(code, N + 3, N + 4, MAKE_SP(N) | MAKE_SP(N + 3));
                RELEASE_1x1(code, N + 3, N, MAKE_SP(N + 3) | MAKE_SP(N + 4));
            } else if (N % 4 >= 2 && block == BLOCK_fill && CAPTURE(code, N + 2) == BLOCK_1x2) {
                RELEASE_1x2(code, N + 2, N + 3, MAKE_SP(N) | MAKE_SP(N + 2));
            }
        }
    } while (false);

    do { // case right
        if (N % 4 < 3 && MAYBE_RIGHT(hint, N)) {
            if (const uint8_t block = CAPTURE(code, N + 1); block == BLOCK_1x1) { // up part
                RELEASE_1x1(code, N + 1, N, MAKE_SP(N + 4) | MAKE_SP(N + 1));
                RELEASE_1x1(code, N + 1, N + 4, MAKE_SP(N) | MAKE_SP(N + 1));
            } else if (N % 4 < 2 && block == BLOCK_1x2) {
                RELEASE_1x2(code, N + 1, N, MAKE_SP(N + 2) | MAKE_SP(N + 4));
            } else if (block == BLOCK_2x1) {
                RELEASE_2x1(code, N + 1, N, MAKE_SP(N + 1) | MAKE_SP(N + 5));
                break;
            } else if (N % 4 < 2 && block == BLOCK_2x2) {
                RELEASE_2x2(code, N + 1, N, MAKE_SP(N + 2) | MAKE_SP(N + 6));
                break;
            }
            if (!MAYBE_RIGHT(hint, N + 4)) {
                break;
            }
            if (const uint8_t block = CAPTURE(code, N + 5); block == BLOCK_1x1) { // down part
                RELEASE_1x1(code, N + 5, N + 4, MAKE_SP(N) | MAKE_SP(N + 5));
                RELEASE_1x1(code, N + 5, N, MAKE_SP(N + 4) | MAKE_SP(N + 5));
            } else if (N % 4 < 2 && block == BLOCK_1x2) {
                RELEASE_1x2(code, N + 5, N + 4, MAKE_SP(N) | MAKE_SP(N + 6));
            }
        }
    } while (false);
}

void S2HintMoverPro::move_double_h(const uint64_t code, uint32_t hint, const int offset) const {
    switch (offset) {
        case 0: move_double_h<0>(code, hint); break;
        case 1: move_double_h<1>(code, hint); break;
        case 2: move_double_h<2>(code, hint); break;
        case 4: move_double_h<4>(code, hint); break;
        case 5: move_double_h<5>(code, hint); break;
        case 6: move_double_h<6>(code, hint); break;
        case 8: move_double_h<8>(code, hint); break;
        case 9: move_double_h<9>(code, hint); break;
        case 10: move_double_h<10>(code, hint); break;
        case 12: move_double_h<12>(code, hint); break;
        case 13: move_double_h<13>(code, hint); break;
        case 14: move_double_h<14>(code, hint); break;
        case 16: move_double_h<16>(code, hint); break;
        case 17: move_double_h<17>(code, hint); break;
        case 18: move_double_h<18>(code, hint); break;
        default: std::unreachable();
    }
}

void S2HintMoverPro::move_double_v(const uint64_t code, uint32_t hint, const int offset) const {
    switch (offset) {
        case 0: move_double_v<0>(code, hint); break;
        case 1: move_double_v<1>(code, hint); break;
        case 2: move_double_v<2>(code, hint); break;
        case 3: move_double_v<3>(code, hint); break;
        case 4: move_double_v<4>(code, hint); break;
        case 5: move_double_v<5>(code, hint); break;
        case 6: move_double_v<6>(code, hint); break;
        case 7: move_double_v<7>(code, hint); break;
        case 8: move_double_v<8>(code, hint); break;
        case 9: move_double_v<9>(code, hint); break;
        case 10: move_double_v<10>(code, hint); break;
        case 11: move_double_v<11>(code, hint); break;
        case 12: move_double_v<12>(code, hint); break;
        case 13: move_double_v<13>(code, hint); break;
        case 14: move_double_v<14>(code, hint); break;
        case 15: move_double_v<15>(code, hint); break;
        default: std::unreachable();
    }
}

void S2HintMoverPro::move_single(const uint64_t code, uint32_t hint, const int offset, int offset_) const {
    uint64_t kk = MAKE_SP(offset_);
    switch (offset) {
        case 0: move_single<0>(code, hint, kk); break;
        case 1: move_single<1>(code, hint, kk); break;
        case 2: move_single<2>(code, hint, kk); break;
        case 3: move_single<3>(code, hint, kk); break;
        case 4: move_single<4>(code, hint, kk); break;
        case 5: move_single<5>(code, hint, kk); break;
        case 6: move_single<6>(code, hint, kk); break;
        case 7: move_single<7>(code, hint, kk); break;
        case 8: move_single<8>(code, hint, kk); break;
        case 9: move_single<9>(code, hint, kk); break;
        case 10: move_single<10>(code, hint, kk); break;
        case 11: move_single<11>(code, hint, kk); break;
        case 12: move_single<12>(code, hint, kk); break;
        case 13: move_single<13>(code, hint, kk); break;
        case 14: move_single<14>(code, hint, kk); break;
        case 15: move_single<15>(code, hint, kk); break;
        case 16: move_single<16>(code, hint, kk); break;
        case 17: move_single<17>(code, hint, kk); break;
        case 18: move_single<18>(code, hint, kk); break;
        case 19: move_single<19>(code, hint, kk); break;
        default: std::unreachable();
    }
}

void S2HintMoverPro::next_cases(const uint64_t code, const uint64_t hint) const {
    // const uint64_t mask = code | (code >> 1) | (code >> 2) | 0xFDB6DB6DB6DB6DB6;
    // const int space_a = std::countr_one(mask) / 3;
    // const int space_b = (63 - std::countl_one(mask)) / 3;

    const uint32_t tmp = hint;
    const int space_a_ = std::countr_zero(tmp);
    const int space_b_ = 31 - std::countl_zero(tmp);

    // if (space_a != space_a_) {
    //     std::cout << "error: " << RawCode::unsafe_create(code).to_common_code() << std::endl;
    // }
    // if (space_b != space_b_) {
    //     std::cout << "error" << RawCode::unsafe_create(code).to_common_code() << std::endl;
    // }

    const uint32_t hint_ = hint >> 32;
    if (space_a_ + 1 == space_b_ && space_a_ % 4 != 3) {
        move_double_h(code, hint_, space_a_);
    } else if (space_a_ + 4 == space_b_) {
        move_double_v(code, hint_, space_a_);
    } else {
        move_single(code, hint_, space_a_, space_b_);
        move_single(code, hint_, space_b_, space_a_);
    }
}

uint64_t S2HintMoverPro::make_hint(const uint64_t code) {
    const uint64_t mask = code | (code >> 1) | (code >> 2) | 0xFDB6DB6DB6DB6DB6;
    const int space_a = std::countr_one(mask) / 3;
    const int space_b = (63 - std::countl_one(mask)) / 3;
    return MAKE_SP(space_a) | MAKE_SP(space_b);
}
