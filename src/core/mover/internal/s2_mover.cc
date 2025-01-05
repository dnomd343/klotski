#include <iostream>

#include "mover/s2_mover.h"
#include "raw_code/raw_code.h"

using klotski::mover::S2Mover;
using klotski::codec::RawCode;

#define CAPTURE(code, addr) \
    (((code) >> ((addr) * 3)) & (uint64_t)(0b111))

#define UNSET_1x1(code, addr) \
    ((code) & ~((uint64_t)0b111 << ((addr) * 3)))

#define SET_1x1(code, addr) \
    ((code) | ((uint64_t)0b011 << ((addr) * 3)))

#define UNSET_1x2(code, addr) \
    ((code) & ~((uint64_t)0b111'111 << ((addr) * 3)))

#define SET_1x2(code, addr) \
    ((code) | ((uint64_t)0b111'001 << ((addr) * 3)))

#define UNSET_2x1(code, addr) \
    ((code) & ~((uint64_t)0b111'000'000'000'111 << ((addr) * 3)))

#define SET_2x1(code, addr) \
    ((code) | ((uint64_t)0b111'000'000'000'010 << ((addr) * 3)))

#define UNSET_2x2(code, addr) \
    ((code) & ~((uint64_t)0b111'111'000'000'111'111 << ((addr) * 3)))

#define SET_2x2(code, addr) \
    ((code) | ((uint64_t)0b111'111'000'000'111'100 << ((addr) * 3)))

template <int N>
void S2Mover::two_space_a(uint64_t code) const {
    // ---------------- case up ----------------

    do {
        if (N >= 4) {
            const uint8_t up_c = CAPTURE(code, N - 4);
            if (up_c == BLOCK_1x2) {
                release_(SET_1x2(UNSET_1x2(code, N - 4), N));
                break;
            }
            if (up_c == BLOCK_1x1) {
                const auto tmp = UNSET_1x1(code, N - 4);
                release_(SET_1x1(tmp, N));
                release_(SET_1x1(tmp, N + 1));
                // -> check right
            } else if (up_c == BLOCK_fill) {
                if (N >= 8) {
                    const uint8_t up_a = CAPTURE(code, N - 8);
                    if (up_a == BLOCK_2x2) {
                        release_(SET_2x2(UNSET_2x2(code, N - 8), N - 4));
                        break;
                    }
                    if (up_a == BLOCK_2x1) {
                        release_(SET_2x1(UNSET_2x1(code, N - 8), N - 4));
                        // -> check right
                    }
                }
                // -> check right
            } else {
                std::unreachable();
            }

            const uint8_t up_d = CAPTURE(code, N - 3);
            if (up_d == BLOCK_1x1) {
                const auto tmp = UNSET_1x1(code, N - 3);
                release_(SET_1x1(tmp, N + 1));
                release_(SET_1x1(tmp, N));
                break;
            }
            if (up_d == BLOCK_fill) {
                if (N >= 8) {
                    const uint8_t up_b = CAPTURE(code, N - 7);
                    if (up_b == BLOCK_2x1) {
                        release_(SET_2x1(UNSET_2x1(code, N - 7), N - 3));
                        break;
                    }
                }
            }
        }
    } while (false);

    // ---------------- case down ----------------

    do {
        if constexpr(N < 16) {
            if (const uint8_t block = CAPTURE(code, N + 4); block == BLOCK_1x1) {
                const auto tmp = UNSET_1x1(code, N + 4);
                release_(SET_1x1(tmp, N));
                release_(SET_1x1(tmp, N + 1));
            } else if (block == BLOCK_1x2) {
                release_(SET_1x2(UNSET_1x2(code, N + 4), N));
                break;
            } else if constexpr(N < 12) {
                if (block == BLOCK_2x1) {
                    release_(SET_2x1(UNSET_2x1(code, N + 4), N));
                } else if (block == BLOCK_2x2) {
                    release_(SET_2x2(UNSET_2x2(code, N + 4), N));
                    break;
                }
            }
            if (const uint8_t block = CAPTURE(code, N + 5); block == BLOCK_1x1) {
                const auto tmp = UNSET_1x1(code, N + 5);
                release_(SET_1x1(tmp, N + 1));
                release_(SET_1x1(tmp, N));
            } else if constexpr(N < 12) {
                if (block == BLOCK_2x1) {
                    release_(SET_2x1(UNSET_2x1(code, N + 5), N + 1));
                }
            }
        }
    } while (false);

    // ---------------- case left ----------------

    if constexpr(N % 4 != 0) {
        if (CAPTURE(code, N - 1) == BLOCK_1x1) {
            release_(SET_1x1(UNSET_1x1(code, N - 1), N));
            release_(SET_1x1(UNSET_1x1(code, N - 1), N + 1));
        } else if constexpr(N % 4 == 2) {
            if (CAPTURE(code, N - 2) == BLOCK_1x2) {
                release_(SET_1x2(UNSET_1x2(code, N - 2), N - 1));
                release_(SET_1x2(UNSET_1x2(code, N - 2), N));
            }
        }
    }

    // ---------------- case right ----------------

    if constexpr(N % 4 != 2) {
        const uint8_t block = CAPTURE(code, N + 2);
        if (block == BLOCK_1x1) {
            release_(SET_1x1(UNSET_1x1(code, N + 2), N + 1));
            release_(SET_1x1(UNSET_1x1(code, N + 2), N));
        } else if constexpr(N % 4 == 0) {
            if (block == BLOCK_1x2) {
                release_(SET_1x2(UNSET_1x2(code, N + 2), N + 1));
                release_(SET_1x2(UNSET_1x2(code, N + 2), N));
            }
        }
    }
}

template <int N>
void S2Mover::two_space_b(uint64_t code) const {
    // ---------------- case up ----------------

    do {
        if (N >= 4) {
            if (CAPTURE(code, N - 4) == BLOCK_1x1) {
                release_(SET_1x1(UNSET_1x1(code, N - 4), N));
                release_(SET_1x1(UNSET_1x1(code, N - 4), N + 4));
                break;
            }
        }
        if (N >= 8) {
            if (CAPTURE(code, N - 8) == BLOCK_2x1) {
                release_(SET_2x1(UNSET_2x1(code, N - 8), N - 4));
                release_(SET_2x1(UNSET_2x1(code, N - 8), N));
                break;
            }
        }
    } while (false);

    // ---------------- case down ----------------

    do {
        if (N < 12) {
            if (CAPTURE(code, N + 8) == BLOCK_1x1) {
                release_(SET_1x1(UNSET_1x1(code, N + 8), N + 4));
                release_(SET_1x1(UNSET_1x1(code, N + 8), N));
                break;
            }
        }
        if (N < 8) {
            if (CAPTURE(code, N + 8) == BLOCK_2x1) {
                release_(SET_2x1(UNSET_2x1(code, N + 8), N + 4));
                release_(SET_2x1(UNSET_2x1(code, N + 8), N));
                break;
            }
        }
    } while (false);

    // ---------------- case left ----------------

    do {
        if ((N % 4) != 0) {
            const uint8_t left_b = CAPTURE(code, N - 1);
            if (left_b == BLOCK_2x1) {
                release_(SET_2x1(UNSET_2x1(code, N - 1), N));
                break;
            }
            if (left_b == BLOCK_1x1) {
                const auto tmp = UNSET_1x1(code, N - 1);
                release_(SET_1x1(tmp, N));
                release_(SET_1x1(tmp, N + 4));
                // -> check down
            } else if (left_b == BLOCK_fill) {
                if ((N % 4) >= 2) {
                    const uint8_t left_d = CAPTURE(code, N - 2);
                    if (left_d == BLOCK_2x2) {
                        release_(SET_2x2(UNSET_2x2(code, N - 2), N - 1));
                        break;
                    }
                    if (left_d == BLOCK_1x2) {
                        release_(SET_1x2(UNSET_1x2(code, N - 2), N - 1));
                        // -> check down
                    }
                }
                // -> check down
            } else {
                std::unreachable();
            }

            const uint8_t left_d = CAPTURE(code, N + 3);
            if (left_d == BLOCK_1x1) {
                const auto tmp = UNSET_1x1(code, N + 3);
                release_(SET_1x1(tmp, N + 4));
                release_(SET_1x1(tmp, N));
                break;
            }
            if (left_d == BLOCK_fill) {
                if ((N % 4) >= 2) {
                    const uint8_t left_c = CAPTURE(code, N + 2);
                    if (left_c == BLOCK_1x2) {
                        release_(SET_1x2(UNSET_1x2(code, N + 2), N + 3));
                        break;
                    }
                }
            }
        }
    } while (false);

    // ---------------- case right ----------------

    do {
        if ((N % 4) != 3) {
            const uint8_t right_a = CAPTURE(code, N + 1);
            if (right_a == BLOCK_2x1) {
                release_(SET_2x1(UNSET_2x1(code, N + 1), N));
                break;
            }
            if (right_a == BLOCK_2x2) {
                release_(SET_2x2(UNSET_2x2(code, N + 1), N));
                break;
            }
            if (right_a == BLOCK_1x2) {
                release_(SET_1x2(UNSET_1x2(code, N + 1), N));
            } else if (right_a == BLOCK_1x1) {
                const auto tmp = UNSET_1x1(code, N + 1);
                release_(SET_1x1(tmp, N));
                release_(SET_1x1(tmp, N + 4));
            }

            const uint8_t right_c = CAPTURE(code, N + 5);
            if (right_c == BLOCK_1x1) {
                const auto tmp = UNSET_1x1(code, N + 5);
                release_(SET_1x1(tmp, N + 4));
                release_(SET_1x1(tmp, N));
                break;
            }
            if (right_c == BLOCK_1x2) {
                release_(SET_1x2(UNSET_1x2(code, N + 5), N + 4));
                break;
            }
        }
    } while (false);
}

template <int N>
void S2Mover::one_space(uint64_t code) const {
    if constexpr(N >= 4) { // case up
        if (CAPTURE(code, N - 4) == BLOCK_1x1) {
            release_(SET_1x1(UNSET_1x1(code, N - 4), N));
        } else if constexpr(N >= 8) {
            if (CAPTURE(code, N - 8) == BLOCK_2x1) {
                release_(SET_2x1(UNSET_2x1(code, N - 8), N - 4));
            }
        }
    }

    if constexpr(N < 16) { // case down
        const uint8_t block = CAPTURE(code, N + 4);
        if (block == BLOCK_1x1) {
            release_(SET_1x1(UNSET_1x1(code, N + 4), N));
        } else if constexpr(N < 12) {
            if (block == BLOCK_2x1) {
                release_(SET_2x1(UNSET_2x1(code, N + 4), N));
            }
        }
    }

    if constexpr(N % 4 != 0) { // case left
        if (CAPTURE(code, N - 1) == BLOCK_1x1) {
            release_(SET_1x1(UNSET_1x1(code, N - 1), N));
        } else if constexpr(N % 4 >= 2) {
            if (CAPTURE(code, N - 2) == BLOCK_1x2) {
                release_(SET_1x2(UNSET_1x2(code, N - 2), N - 1));
            }
        }
    }

    if constexpr(N % 4 != 3) { // case right
        const uint8_t block = CAPTURE(code, N + 1);
        if (block == BLOCK_1x1) {
            release_(SET_1x1(UNSET_1x1(code, N + 1), N));
        } else if constexpr(N % 4 <= 1) {
            if (block == BLOCK_1x2) {
                release_(SET_1x2(UNSET_1x2(code, N + 1), N));
            }
        }
    }
}

void S2Mover::two_space_a_(uint64_t code, int offset) const {
    switch (offset) {
    case 0: two_space_a<0>(code); break;
    case 1: two_space_a<1>(code); break;
    case 2: two_space_a<2>(code); break;
    case 4: two_space_a<4>(code); break;
    case 5: two_space_a<5>(code); break;
    case 6: two_space_a<6>(code); break;
    case 8: two_space_a<8>(code); break;
    case 9: two_space_a<9>(code); break;
    case 10: two_space_a<10>(code); break;
    case 12: two_space_a<12>(code); break;
    case 13: two_space_a<13>(code); break;
    case 14: two_space_a<14>(code); break;
    case 16: two_space_a<16>(code); break;
    case 17: two_space_a<17>(code); break;
    case 18: two_space_a<18>(code); break;
    default: std::unreachable();
    }
}

void S2Mover::two_space_b_(uint64_t code, int offset) const {
    switch (offset) {
    case 0: two_space_b<0>(code); break;
    case 1: two_space_b<1>(code); break;
    case 2: two_space_b<2>(code); break;
    case 3: two_space_b<3>(code); break;
    case 4: two_space_b<4>(code); break;
    case 5: two_space_b<5>(code); break;
    case 6: two_space_b<6>(code); break;
    case 7: two_space_b<7>(code); break;
    case 8: two_space_b<8>(code); break;
    case 9: two_space_b<9>(code); break;
    case 10: two_space_b<10>(code); break;
    case 11: two_space_b<11>(code); break;
    case 12: two_space_b<12>(code); break;
    case 13: two_space_b<13>(code); break;
    case 14: two_space_b<14>(code); break;
    case 15: two_space_b<15>(code); break;
    default: std::unreachable();
    }
}

void S2Mover::one_space_(uint64_t code, int offset) const {
    switch (offset) {
    case 0: one_space<0>(code); break;
    case 1: one_space<1>(code); break;
    case 2: one_space<2>(code); break;
    case 3: one_space<3>(code); break;
    case 4: one_space<4>(code); break;
    case 5: one_space<5>(code); break;
    case 6: one_space<6>(code); break;
    case 7: one_space<7>(code); break;
    case 8: one_space<8>(code); break;
    case 9: one_space<9>(code); break;
    case 10: one_space<10>(code); break;
    case 11: one_space<11>(code); break;
    case 12: one_space<12>(code); break;
    case 13: one_space<13>(code); break;
    case 14: one_space<14>(code); break;
    case 15: one_space<15>(code); break;
    case 16: one_space<16>(code); break;
    case 17: one_space<17>(code); break;
    case 18: one_space<18>(code); break;
    case 19: one_space<19>(code); break;
    default: std::unreachable();
    }
}

void S2Mover::next_cases(uint64_t code) {
    int space_1 = -1;
    int space_2 = -1;
    for (int addr = 0; addr < 20; ++addr) {
        if (((code >> (addr * 3)) & 0b111) == 0) {
            if (space_1 == -1) {
                space_1 = addr;
                continue;
            }
            space_2 = addr;
        }
    }

    if (space_1 + 1 == space_2 && space_1 % 4 != 3) {
        two_space_a_(code, space_1);
    } else if (space_1 + 4 == space_2) {
        two_space_b_(code, space_1);
    } else {
        one_space_(code, space_1);
        one_space_(code, space_2);
    }
}
