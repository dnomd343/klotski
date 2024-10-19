#include "mover/mover.h"
#include "group/group.h"
#include "constant/group_union.h"

using klotski::codec::RawCode;
using klotski::codec::CommonCode;
using klotski::cases::GroupUnion;

#define RANGE_DERIVE(HEAD) ranges.derive(HEAD, cases[HEAD])

static KLSK_INLINE uint32_t to_type_id(const int n, const int n_2x1, const int n_1x1) {
    constexpr int offset[8] = {0, 15, 41, 74, 110, 145, 175, 196};
    return offset[n] + (15 - n * 2) * n_2x1 + n_1x1;
}

uint32_t GroupUnion::type_id(const CommonCode common_code) {
    const auto range = static_cast<uint32_t>(common_code.unwrap());
    const auto n_1x1 = std::popcount((range >> 1) & range & 0x55555555);
    const auto n_2x1 = std::popcount((range >> 1) & ~range & 0x55555555);
    return to_type_id(std::popcount(range) - n_1x1 * 2, n_2x1, n_1x1);
}

uint32_t GroupUnion::type_id(const RawCode raw_code) {
    const auto code = raw_code.unwrap();
    const auto n = std::popcount(((code >> 1) ^ code) & 0x0249249249249249);
    const auto n_2x1 = std::popcount((code >> 1) & ~code & 0x0249249249249249);
    const auto n_1x1 = std::popcount((code >> 1) & code & 0x0249249249249249) - n - 3;
    return to_type_id(n, n_2x1, n_1x1);
}

klotski::cases::RangesUnion GroupUnion::cases() const {
    auto [n, n_2x1, n_1x1] = BLOCK_NUM[type_id_];
    auto [s_a, s_b, s_c, s_d] = GROUP_UNION_CASES_NUM[type_id_];

    Ranges ranges {};
    ranges.reserve(BASIC_RANGES_NUM[type_id_]);
    ranges.spawn(n, n_2x1, n_1x1);
    ranges.reverse();

    RangesUnion cases;
    cases[0x0].reserve(s_a); cases[0x1].reserve(s_b); cases[0x2].reserve(s_a);
    cases[0x4].reserve(s_c); cases[0x5].reserve(s_d); cases[0x6].reserve(s_c);
    cases[0x8].reserve(s_c); cases[0x9].reserve(s_d); cases[0xA].reserve(s_c);
    cases[0xC].reserve(s_a); cases[0xD].reserve(s_b); cases[0xE].reserve(s_a);

    RANGE_DERIVE(0x0); RANGE_DERIVE(0x1); RANGE_DERIVE(0x2);
    RANGE_DERIVE(0x4); RANGE_DERIVE(0x5); RANGE_DERIVE(0x6);
    RANGE_DERIVE(0x8); RANGE_DERIVE(0x9); RANGE_DERIVE(0xA);
    RANGE_DERIVE(0xC); RANGE_DERIVE(0xD); RANGE_DERIVE(0xE);
    return cases;
}
