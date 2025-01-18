#include "mover/mover.h"
#include "raw_code/raw_code.h"

using klotski::codec::RawCode;
using klotski::mover::S2HintMover;

// TODO: using `CAPTURE(N + xxx)` instead of `CAPTURE(code, N + xxx)`
//       also for `RELEASE_xxx` and `MAYBE_xxx`

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

#define RELEASE_1x1(code, SRC, DST) \
    release_(RawCode::unsafe_create(MOVE_1x1(code, SRC, DST)), (0b1 << (DST)))

#define RELEASE_1x2(code, SRC, DST) \
    release_(RawCode::unsafe_create(MOVE_1x2(code, SRC, DST)), (0b11 << (DST)))

#define RELEASE_2x1(code, SRC, DST) \
    release_(RawCode::unsafe_create(MOVE_2x1(code, SRC, DST)), (0b10001 << (DST)))

#define RELEASE_2x2(code, SRC, DST) \
    release_(RawCode::unsafe_create(MOVE_2x2(code, SRC, DST)), (0b110011 << (DST)))

#define MAYBE_UP(hint, N) (((hint) & (1 << ((N) - 4))) == 0)

#define MAYBE_DOWN(hint, N) (((hint) & (1 << ((N) + 4))) == 0)

#define MAYBE_LEFT(hint, N) (((hint) & (1 << ((N) - 1))) == 0)

#define MAYBE_RIGHT(hint, N) (((hint) & (1 << ((N) + 1))) == 0)

template <int N>
void S2HintMover::move_single(const uint64_t code, const uint64_t hint) const {
    if (N >= 4 && MAYBE_UP(hint, N)) { // case up
        if (CAPTURE(code, N - 4) == BLOCK_1x1) {
            RELEASE_1x1(code, N - 4, N);
        } else if (N >= 8 && CAPTURE(code, N - 8) == BLOCK_2x1) {
            RELEASE_2x1(code, N - 8, N - 4);
        }
    }

    if (N < 16 && MAYBE_DOWN(hint, N)) { // case down
        if (const uint8_t block = CAPTURE(code, N + 4); block == BLOCK_1x1) {
            RELEASE_1x1(code, N + 4, N);
        } else if (N < 12 && block == BLOCK_2x1) {
            RELEASE_2x1(code, N + 4, N);
        }
    }

    if (N % 4 >= 1 && MAYBE_LEFT(hint, N)) { // case left
        if (CAPTURE(code, N - 1) == BLOCK_1x1) {
            RELEASE_1x1(code, N - 1, N);
        } else if (N % 4 >= 2 && CAPTURE(code, N - 2) == BLOCK_1x2) {
            RELEASE_1x2(code, N - 2, N - 1);
        }
    }

    if (N % 4 < 3 && MAYBE_RIGHT(hint, N)) { // case right
        if (const uint8_t block = CAPTURE(code, N + 1); block == BLOCK_1x1) {
            RELEASE_1x1(code, N + 1, N);
        } else if (N % 4 < 2 && block == BLOCK_1x2) {
            RELEASE_1x2(code, N + 1, N);
        }
    }
}

template <int N>
void S2HintMover::move_double_h(const uint64_t code, uint64_t hint) const {
    do { // case up
        if (N >= 4 && MAYBE_UP(hint, N)) {
            if (const uint8_t block = CAPTURE(code, N - 4); block == BLOCK_1x1) { // left part
                RELEASE_1x1(code, N - 4, N);
                RELEASE_1x1(code, N - 4, N + 1);
            } else if (block == BLOCK_1x2) {
                RELEASE_1x2(code, N - 4, N);
                break;
            } else if (N >= 8 && block == BLOCK_fill) {
                if (const uint8_t block_ = CAPTURE(code, N - 8); block_ == BLOCK_2x2) {
                    RELEASE_2x2(code, N - 8, N - 4);
                    break;
                } else if (block_ == BLOCK_2x1) {
                    RELEASE_2x1(code, N - 8, N - 4);
                }
            }
            if (!MAYBE_UP(hint, N + 1)) {
                break;
            }
            if (const uint8_t block = CAPTURE(code, N - 3); block == BLOCK_1x1) { // right part
                RELEASE_1x1(code, N - 3, N + 1);
                RELEASE_1x1(code, N - 3, N);
            } else if (N >= 8 && block == BLOCK_fill && CAPTURE(code, N - 7) == BLOCK_2x1) {
                RELEASE_2x1(code, N - 7, N - 3);
            }
        }
    } while (false);

    do { // case down
        if (N < 16 && MAYBE_DOWN(hint, N)) {
            if (const uint8_t block = CAPTURE(code, N + 4); block == BLOCK_1x1) { // left part
                RELEASE_1x1(code, N + 4, N);
                RELEASE_1x1(code, N + 4, N + 1);
            } else if (N < 12 && block == BLOCK_2x1) {
                RELEASE_2x1(code, N + 4, N);
            } else if (block == BLOCK_1x2) {
                RELEASE_1x2(code, N + 4, N);
                break;
            } else if (N < 12 && block == BLOCK_2x2) {
                RELEASE_2x2(code, N + 4, N);
                break;
            }
            if (!MAYBE_DOWN(hint, N + 1)) {
                break;
            }
            if (const uint8_t block = CAPTURE(code, N + 5); block == BLOCK_1x1) { // right part
                RELEASE_1x1(code, N + 5, N + 1);
                RELEASE_1x1(code, N + 5, N);
            } else if (N < 12 && block == BLOCK_2x1) {
                RELEASE_2x1(code, N + 5, N + 1);
            }
        }
    } while (false);

    if (N % 4 >= 1 && MAYBE_LEFT(hint, N)) { // case left
        if (CAPTURE(code, N - 1) == BLOCK_1x1) {
            RELEASE_1x1(code, N - 1, N);
            RELEASE_1x1(code, N - 1, N + 1);
        } else if (N % 4 == 2 && CAPTURE(code, N - 2) == BLOCK_1x2) {
            RELEASE_1x2(code, N - 2, N - 1);
            RELEASE_1x2(code, N - 2, N);
        }
    }

    if (N % 4 < 2 && MAYBE_RIGHT(hint, N + 1)) { // case right
        if (const uint8_t block = CAPTURE(code, N + 2); block == BLOCK_1x1) {
            RELEASE_1x1(code, N + 2, N + 1);
            RELEASE_1x1(code, N + 2, N);
        } else if (N % 4 == 0 && block == BLOCK_1x2) {
            RELEASE_1x2(code, N + 2, N + 1);
            RELEASE_1x2(code, N + 2, N);
        }
    }
}

template <int N>
void S2HintMover::move_double_v(const uint64_t code, uint64_t hint) const {
    if (N >= 4 && MAYBE_UP(hint, N)) { // case up
        if (CAPTURE(code, N - 4) == BLOCK_1x1) {
            RELEASE_1x1(code, N - 4, N);
            RELEASE_1x1(code, N - 4, N + 4);
        } else if (N >= 8 && CAPTURE(code, N - 8) == BLOCK_2x1) {
            RELEASE_2x1(code, N - 8, N - 4);
            RELEASE_2x1(code, N - 8, N);
        }
    }

    if (N < 12 && MAYBE_DOWN(hint, N + 4)) { // case down
        if (CAPTURE(code, N + 8) == BLOCK_1x1) {
            RELEASE_1x1(code, N + 8, N + 4);
            RELEASE_1x1(code, N + 8, N);
        } else if (N < 8 && CAPTURE(code, N + 8) == BLOCK_2x1) {
            RELEASE_2x1(code, N + 8, N + 4);
            RELEASE_2x1(code, N + 8, N);
        }
    }

    do { // case left
        if (N % 4 != 0 && MAYBE_LEFT(hint, N)) {
            if (const uint8_t block = CAPTURE(code, N - 1); block == BLOCK_1x1) { // up part
                RELEASE_1x1(code, N - 1, N);
                RELEASE_1x1(code, N - 1, N + 4);
            } else if (block == BLOCK_2x1) {
                RELEASE_2x1(code, N - 1, N);
                break;
            } else if (N % 4 >= 2 && block == BLOCK_fill) {
                if (const uint8_t block_ = CAPTURE(code, N - 2); block_ == BLOCK_2x2) {
                    RELEASE_2x2(code, N - 2, N - 1);
                    break;
                } else if (block_ == BLOCK_1x2) {
                    RELEASE_1x2(code, N - 2, N - 1);
                }
            }
            if (!MAYBE_LEFT(hint, N + 4)) {
                break;
            }
            if (const uint8_t block = CAPTURE(code, N + 3); block == BLOCK_1x1) { // down part
                RELEASE_1x1(code, N + 3, N + 4);
                RELEASE_1x1(code, N + 3, N);
            } else if (N % 4 >= 2 && block == BLOCK_fill && CAPTURE(code, N + 2) == BLOCK_1x2) {
                RELEASE_1x2(code, N + 2, N + 3);
            }
        }
    } while (false);

    do { // case right
        if (N % 4 < 3 && MAYBE_RIGHT(hint, N)) {
            if (const uint8_t block = CAPTURE(code, N + 1); block == BLOCK_1x1) { // up part
                RELEASE_1x1(code, N + 1, N);
                RELEASE_1x1(code, N + 1, N + 4);
            } else if (N % 4 < 2 && block == BLOCK_1x2) {
                RELEASE_1x2(code, N + 1, N);
            } else if (block == BLOCK_2x1) {
                RELEASE_2x1(code, N + 1, N);
                break;
            } else if (N % 4 < 2 && block == BLOCK_2x2) {
                RELEASE_2x2(code, N + 1, N);
                break;
            }
            if (!MAYBE_RIGHT(hint, N + 4)) {
                break;
            }
            if (const uint8_t block = CAPTURE(code, N + 5); block == BLOCK_1x1) { // down part
                RELEASE_1x1(code, N + 5, N + 4);
                RELEASE_1x1(code, N + 5, N);
            } else if (N % 4 < 2 && block == BLOCK_1x2) {
                RELEASE_1x2(code, N + 5, N + 4);
            }
        }
    } while (false);
}

void S2HintMover::move_double_h(const uint64_t code, uint64_t hint, const int offset) const {
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

void S2HintMover::move_double_v(const uint64_t code, uint64_t hint, const int offset) const {
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

void S2HintMover::move_single(const uint64_t code, uint64_t hint, const int offset) const {
    switch (offset) {
        case 0: move_single<0>(code, hint); break;
        case 1: move_single<1>(code, hint); break;
        case 2: move_single<2>(code, hint); break;
        case 3: move_single<3>(code, hint); break;
        case 4: move_single<4>(code, hint); break;
        case 5: move_single<5>(code, hint); break;
        case 6: move_single<6>(code, hint); break;
        case 7: move_single<7>(code, hint); break;
        case 8: move_single<8>(code, hint); break;
        case 9: move_single<9>(code, hint); break;
        case 10: move_single<10>(code, hint); break;
        case 11: move_single<11>(code, hint); break;
        case 12: move_single<12>(code, hint); break;
        case 13: move_single<13>(code, hint); break;
        case 14: move_single<14>(code, hint); break;
        case 15: move_single<15>(code, hint); break;
        case 16: move_single<16>(code, hint); break;
        case 17: move_single<17>(code, hint); break;
        case 18: move_single<18>(code, hint); break;
        case 19: move_single<19>(code, hint); break;
        default: std::unreachable();
    }
}

void S2HintMover::next_cases(const RawCode code, const uint64_t hint) const {
    uint64_t code_ = code.unwrap();
    const uint64_t mask = code_ | (code_ >> 1) | (code_ >> 2) | 0xFDB6DB6DB6DB6DB6;
    const int space_a = std::countr_one(mask) / 3;
    const int space_b = (63 - std::countl_one(mask)) / 3;

    if (space_a + 1 == space_b && space_a % 4 != 3) {
        move_double_h(code_, hint, space_a);
    } else if (space_a + 4 == space_b) {
        move_double_v(code_, hint, space_a);
    } else {
        move_single(code_, hint, space_a);
        move_single(code_, hint, space_b);
    }
}