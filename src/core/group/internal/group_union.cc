#include "group/group.h"

using klotski::cases::Ranges;
using klotski::group::GroupUnion;
using klotski::cases::RangesUnion;

using klotski::cases::BASIC_RANGES_NUM;

#define RANGE_DERIVE(HEAD) ranges.derive(HEAD, cases.ranges(HEAD))
#define RANGE_RESERVE(HEAD, SIZE) cases.ranges(HEAD).reserve(SIZE)

RangesUnion GroupUnion::cases() const {
    Ranges ranges {};
    ranges.reserve(BASIC_RANGES_NUM[type_id_]);
    const auto [n, n_2x1, n_1x1] = BLOCK_NUM[type_id_];
    ranges.spawn(n, n_2x1, n_1x1);
    ranges.reverse();

    RangesUnion cases;
    const auto [s_a, s_b, s_c, s_d] = GROUP_UNION_CASES_NUM[type_id_];
    RANGE_RESERVE(0x0, s_a); RANGE_RESERVE(0x1, s_b); RANGE_RESERVE(0x2, s_a);
    RANGE_RESERVE(0x4, s_c); RANGE_RESERVE(0x5, s_d); RANGE_RESERVE(0x6, s_c);
    RANGE_RESERVE(0x8, s_c); RANGE_RESERVE(0x9, s_d); RANGE_RESERVE(0xA, s_c);
    RANGE_RESERVE(0xC, s_a); RANGE_RESERVE(0xD, s_b); RANGE_RESERVE(0xE, s_a);

    RANGE_DERIVE(0x0); RANGE_DERIVE(0x1); RANGE_DERIVE(0x2);
    RANGE_DERIVE(0x4); RANGE_DERIVE(0x5); RANGE_DERIVE(0x6);
    RANGE_DERIVE(0x8); RANGE_DERIVE(0x9); RANGE_DERIVE(0xA);
    RANGE_DERIVE(0xC); RANGE_DERIVE(0xD); RANGE_DERIVE(0xE);
    return cases;
}
