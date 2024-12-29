#include <iostream>

#include "mover/s2_mover.h"
#include "raw_code/raw_code.h"

using klotski::mover::S2Mover;
using klotski::codec::RawCode;

#define APPLY_MASK(code, addr, mask) \
    (((code) >> ((addr) * 3)) & (uint64_t)(mask))

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

void S2Mover::two_space_a(uint64_t code, int offset) const {
    // TODO: only apply once (maybe using goto or do-while)

    // ---------------- case up ----------------

    if (offset >= 4) {
        /// 1x1 & 1x1
        if (APPLY_MASK(code, offset - 4, 0b111'111) == 0b011'011) {
            auto tmp_1 = UNSET_1x1(code, offset - 4);
            release_(SET_1x1(tmp_1, offset));
            release_(SET_1x1(tmp_1, offset + 1));
            auto tmp_2 = UNSET_1x1(code, offset - 3);
            release_(SET_1x1(tmp_2, offset + 1));
            release_(SET_1x1(tmp_2, offset));
        }

        /// 1x2
        if (APPLY_MASK(code, offset - 4, 0b111'111) == 0b111'001) {
            release_(SET_1x2(UNSET_1x2(code, offset - 4), offset));
        }
    }
    if (offset >= 8) {
        /// 1x1 & 2x1
        if (APPLY_MASK(code, offset - 8, 0b111'111'000'000'111'000) == 0b111'011'000'000'010'000) {
            auto tmp_1 = UNSET_1x1(code, offset - 4);
            release_(SET_1x1(tmp_1, offset));
            release_(SET_1x1(tmp_1, offset + 1));
            release_(SET_2x1(UNSET_2x1(code, offset - 7), offset - 3));
        }

        /// 2x1 & 1x1
        if (APPLY_MASK(code, offset - 8, 0b111'111'000'000'000'111) == 0b011'111'000'000'000'010) {
            release_(SET_2x1(UNSET_2x1(code, offset - 8), offset - 4));
            auto tmp_1 = UNSET_1x1(code, offset - 3);
            release_(SET_1x1(tmp_1, offset + 1));
            release_(SET_1x1(tmp_1, offset));
        }

        /// 2x1 & 2x1
        if (APPLY_MASK(code, offset - 8, 0b111'111'000'000'111'111) == 0b111'111'000'000'010'010) {
            release_(SET_2x1(UNSET_2x1(code, offset - 8), offset - 4));
            release_(SET_2x1(UNSET_2x1(code, offset - 7), offset - 3));
        }

        /// 2x2
        if (APPLY_MASK(code, offset - 8, 0b111'111'000'000'111'111) == 0b111'111'000'000'111'100) {
            release_(SET_2x2(UNSET_2x2(code, offset - 8), offset - 4));
        }
    }

    // ---------------- case down ----------------

    if (offset < 16) {
        /// 1x1 & 1x1
        if (APPLY_MASK(code, offset + 4, 0b111'111) == 0b011'011) {
            auto tmp_1 = UNSET_1x1(code, offset + 4);
            release_(SET_1x1(tmp_1, offset));
            release_(SET_1x1(tmp_1, offset + 1));
            auto tmp_2 = UNSET_1x1(code, offset + 5);
            release_(SET_1x1(tmp_2, offset + 1));
            release_(SET_1x1(tmp_2, offset));
        }

        /// 1x2
        if (APPLY_MASK(code, offset + 4, 0b111'111) == 0b111'001) {
            release_(SET_1x2(UNSET_1x2(code, offset + 4), offset));
        }
    }
    if (offset < 12) {
        /// 1x1 & 2x1
        if (APPLY_MASK(code, offset + 4, 0b111'000'000'000'111'111) == 0b111'000'000'000'010'011) {
            auto tmp_1 = UNSET_1x1(code, offset + 4);
            release_(SET_1x1(tmp_1, offset));
            release_(SET_1x1(tmp_1, offset + 1));
            release_(SET_2x1(UNSET_2x1(code, offset + 5), offset + 1));
        }

        /// 2x1 & 1x1
        if (APPLY_MASK(code, offset + 4, 0b000'111'000'000'111'111) == 0b000'111'000'000'011'010) {
            release_(SET_2x1(UNSET_2x1(code, offset + 4), offset));
            auto tmp_1 = UNSET_1x1(code, offset + 5);
            release_(SET_1x1(tmp_1, offset + 1));
            release_(SET_1x1(tmp_1, offset));
        }

        /// 2x1 & 2x1
        if (APPLY_MASK(code, offset + 4, 0b111'111'000'000'111'111) == 0b111'111'000'000'010'010) {
            release_(SET_2x1(UNSET_2x1(code, offset + 4), offset));
            release_(SET_2x1(UNSET_2x1(code, offset + 5), offset + 1));
        }

        /// 2x2
        if (APPLY_MASK(code, offset + 4, 0b111'111'000'000'111'111) == 0b111'111'000'000'111'100) {
            release_(SET_2x2(UNSET_2x2(code, offset + 4), offset));
        }
    }

    // ---------------- case left ----------------

    if ((offset % 4) != 0) {
        /// 1x1
        if (APPLY_MASK(code, offset - 1, 0b111) == 0b011) {
            release_(SET_1x1(UNSET_1x1(code, offset - 1), offset));
            release_(SET_1x1(UNSET_1x1(code, offset - 1), offset + 1));
        }
    }
    if ((offset % 4) == 2) {
        /// 1x2
        if (APPLY_MASK(code, offset - 2, 0b111'111) == 0b111'001) {
            release_(SET_1x2(UNSET_1x2(code, offset - 2), offset - 1));
            release_(SET_1x2(UNSET_1x2(code, offset - 2), offset));
        }
    }

    // ---------------- case right ----------------

    if ((offset % 4) != 2) {
        /// 1x1
        if (APPLY_MASK(code, offset + 2, 0b111) == 0b011) {
            release_(SET_1x1(UNSET_1x1(code, offset + 2), offset + 1));
            release_(SET_1x1(UNSET_1x1(code, offset + 2), offset));
        }
    }
    if ((offset % 4) == 0) {
        /// 1x2
        if (APPLY_MASK(code, offset + 2, 0b111'111) == 0b111'001) {
            release_(SET_1x2(UNSET_1x2(code, offset + 2), offset + 1));
            release_(SET_1x2(UNSET_1x2(code, offset + 2), offset));
        }
    }
}

void S2Mover::two_space_b(uint64_t code, int offset) const {
    // ---------------- case up ----------------

    if (offset >= 4) {
        /// 1x1
        if (APPLY_MASK(code, offset - 4, 0b111) == 0b011) {
            release_(SET_1x1(UNSET_1x1(code, offset - 4), offset));
            release_(SET_1x1(UNSET_1x1(code, offset - 4), offset + 4));
        }
    }
    if (offset >= 8) {
        /// 2x1
        if (APPLY_MASK(code, offset - 8, 0b111'000'000'000'111) == 0b111'000'000'000'010) {
            release_(SET_2x1(UNSET_2x1(code, offset - 8), offset - 4));
            release_(SET_2x1(UNSET_2x1(code, offset - 8), offset));
        }
    }

    // ---------------- case down ----------------

    if (offset < 16) {
        /// 1x1
        if (APPLY_MASK(code, offset + 8, 0b111) == 0b011) {
            release_(SET_1x1(UNSET_1x1(code, offset + 8), offset + 4));
            release_(SET_1x1(UNSET_1x1(code, offset + 8), offset));
        }
    }
    if (offset < 12) {
        /// 2x1
        if (APPLY_MASK(code, offset + 8, 0b111'000'000'000'111) == 0b111'000'000'000'010) {
            release_(SET_2x1(UNSET_2x1(code, offset + 8), offset + 4));
            release_(SET_2x1(UNSET_2x1(code, offset + 8), offset));
        }
    }

    // ---------------- case left ----------------

    if ((offset % 4) != 0) {
        /// 1x1 & 1x1
        if (APPLY_MASK(code, offset - 1, 0b111'000'000'000'111) == 0b011'000'000'000'011) {
            auto tmp_1 = UNSET_1x1(code, offset - 1);
            release_(SET_1x1(tmp_1, offset));
            release_(SET_1x1(tmp_1, offset + 4));
            auto tmp_2 = UNSET_1x1(code, offset + 3);
            release_(SET_1x1(tmp_2, offset + 4));
            release_(SET_1x1(tmp_2, offset));
        }

        /// 2x1
        if (APPLY_MASK(code, offset - 1, 0b111'000'000'000'111) == 0b111'000'000'000'010) {
            release_(SET_2x1(UNSET_2x1(code, offset - 1), offset));
        }
    }
    if ((offset % 4) >= 2) {
        /// 1x1 & 1x2
        if (APPLY_MASK(code, offset - 2, 0b111'111'000'000'111'000) == 0b111'001'000'000'011'000) {
            auto tmp_1 = UNSET_1x1(code, offset - 1);
            release_(SET_1x1(tmp_1, offset));
            release_(SET_1x1(tmp_1, offset + 4));
            release_(SET_1x2(UNSET_1x2(code, offset + 2), offset + 3));
        }

        /// 1x2 & 1x1
        if (APPLY_MASK(code, offset - 2, 0b111'000'000'000'111'111) == 0b011'000'000'000'111'001) {
            release_(SET_1x2(UNSET_1x2(code, offset - 2), offset - 1));
            auto tmp_1 = UNSET_1x1(code, offset + 3);
            release_(SET_1x1(tmp_1, offset + 4));
            release_(SET_1x1(tmp_1, offset));
        }

        /// 1x2 & 1x2
        if (APPLY_MASK(code, offset - 2, 0b111'111'000'000'111'111) == 0b111'001'000'000'111'001) {
            release_(SET_1x2(UNSET_1x2(code, offset - 2), offset - 1));
            release_(SET_1x2(UNSET_1x2(code, offset + 2), offset + 3));
        }

        /// 2x2
        if (APPLY_MASK(code, offset - 2, 0b111'111'000'000'111'111) == 0b111'111'000'000'111'100) {
            release_(SET_2x2(UNSET_2x2(code, offset - 2), offset - 1));
        }
    }

    // ---------------- case right ----------------

    if ((offset % 4) != 3) {
        /// 1x1 & 1x1
        if (APPLY_MASK(code, offset + 1, 0b111'000'000'000'111) == 0b011'000'000'000'011) {
            auto tmp_1 = UNSET_1x1(code, offset + 1);
            release_(SET_1x1(tmp_1, offset));
            release_(SET_1x1(tmp_1, offset + 4));
            auto tmp_2 = UNSET_1x1(code, offset + 5);
            release_(SET_1x1(tmp_2, offset + 4));
            release_(SET_1x1(tmp_2, offset));
        }

        /// 2x1
        if (APPLY_MASK(code, offset + 1, 0b111'000'000'000'111) == 0b111'000'000'000'010) {
            release_(SET_2x1(UNSET_2x1(code, offset + 1), offset));
        }
    }
    if ((offset % 4) <= 1) {
        /// 1x1 & 1x2
        if (APPLY_MASK(code, offset + 1, 0b111'111'000'000'000'111) == 0b111'001'000'000'000'011) {
            auto tmp_1 = UNSET_1x1(code, offset + 1);
            release_(SET_1x1(tmp_1, offset));
            release_(SET_1x1(tmp_1, offset + 4));
            release_(SET_1x2(UNSET_1x2(code, offset + 5), offset + 4));
        }

        /// 1x2 & 1x1
        if (APPLY_MASK(code, offset + 1, 0b000'111'000'000'111'111) == 0b000'011'000'000'111'001) {
            release_(SET_1x2(UNSET_1x2(code, offset + 1), offset));
            auto tmp_1 = UNSET_1x1(code, offset + 5);
            release_(SET_1x1(tmp_1, offset + 4));
            release_(SET_1x1(tmp_1, offset));
        }

        /// 1x2 & 1x2
        if (APPLY_MASK(code, offset + 1, 0b111'111'000'000'111'111) == 0b111'001'000'000'111'001) {
            release_(SET_1x2(UNSET_1x2(code, offset + 1), offset));
            release_(SET_1x2(UNSET_1x2(code, offset + 5), offset + 4));
        }

        /// 2x2
        if (APPLY_MASK(code, offset + 1, 0b111'111'000'000'111'111) == 0b111'111'000'000'111'100) {
            release_(SET_2x2(UNSET_2x2(code, offset + 1), offset));
        }
    }
}

void S2Mover::one_space(uint64_t code, int offset) const {
    // ---------------- case up ----------------

    if (offset >= 4) {
        /// 1x1
        if (APPLY_MASK(code, offset - 4, 0b111) == 0b011) {
            release_(SET_1x1(UNSET_1x1(code, offset - 4), offset));
        }
    }
    if (offset >= 8) {
        /// 2x1
        if (APPLY_MASK(code, offset - 8, 0b111'000'000'000'111) == 0b111'000'000'000'010) {
            release_(SET_2x1(UNSET_2x1(code, offset - 8), offset - 4));
        }
    }

    // ---------------- case down ----------------

    if (offset < 16) {
        /// 1x1
        if (APPLY_MASK(code, offset + 4, 0b111) == 0b011) {
            release_(SET_1x1(UNSET_1x1(code, offset + 4), offset));
        }
    }
    if (offset < 12) {
        /// 2x1
        if (APPLY_MASK(code, offset + 4, 0b111'000'000'000'111) == 0b111'000'000'000'010) {
            release_(SET_2x1(UNSET_2x1(code, offset + 4), offset));
        }
    }

    // ---------------- case left ----------------

    if ((offset % 4) != 0) {
        /// 1x1
        if (APPLY_MASK(code, offset - 1, 0b111) == 0b011) {
            release_(SET_1x1(UNSET_1x1(code, offset - 1), offset));
        }
    }
    if ((offset % 4) >= 2) {
        /// 1x2
        if (APPLY_MASK(code, offset - 2, 0b111'111) == 0b111'001) {
            release_(SET_1x2(UNSET_1x2(code, offset - 2), offset - 1));
        }
    }

    // ---------------- case right ----------------

    if ((offset % 4) != 3) {
        /// 1x1
        if (APPLY_MASK(code, offset + 1, 0b111) == 0b011) {
            release_(SET_1x1(UNSET_1x1(code, offset + 1), offset));
        }
    }
    if ((offset % 4) <= 1) {
        /// 1x2
        if (APPLY_MASK(code, offset + 1, 0b111'111) == 0b111'001) {
            release_(SET_1x2(UNSET_1x2(code, offset + 1), offset));
        }
    }
}

void S2Mover::next_cases(uint64_t code) {
    // std::cout << RawCode::unsafe_create(code) << std::endl;

    // constexpr int space_1 = 17, space_2 = 18;
    // constexpr int space_1 = 1, space_2 = 2;
    // constexpr int space_1 = 18, space_2 = 19;
    // constexpr int space_1 = 16, space_2 = 17;
    // two_space_a(code, space_1);

    // constexpr int space_1 = 12, space_2 = 16;
    // constexpr int space_1 = 0, space_2 = 4;
    // constexpr int space_1 = 15, space_2 = 19;
    // constexpr int space_1 = 12, space_2 = 16;
    // two_space_b(code, space_1);

    // constexpr int space = 12;
    // constexpr int space = 4;
    // constexpr int space = 19;
    // constexpr int space = 16;
    // one_space(code, space);

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
    // std::cout << space_1 << std::endl;
    // std::cout << space_2 << std::endl;

    if (space_1 + 1 == space_2 && space_1 % 4 != 3) {
        two_space_a(code, space_1);
    } else if (space_1 + 4 == space_2) {
        two_space_b(code, space_1);
    } else {
        one_space(code, space_1);
        one_space(code, space_2);
    }
}
