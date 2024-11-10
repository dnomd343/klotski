#include "group/group.h"
#include "constant/group_union.h"

using klotski::cases::Ranges;
using klotski::group::GroupUnion;
using klotski::cases::RangesUnion;

using klotski::cases::BASIC_RANGES_NUM;

#define RANGE_DERIVE(HEAD) ranges.derive(HEAD, cases.ranges(HEAD))

RangesUnion GroupUnion::cases() const {
    auto [n, n_2x1, n_1x1] = BLOCK_NUM[type_id_];
    auto [s_a, s_b, s_c, s_d] = GROUP_UNION_CASES_NUM[type_id_];

    Ranges ranges {};
    ranges.reserve(BASIC_RANGES_NUM[type_id_]);
    ranges.spawn(n, n_2x1, n_1x1);
    ranges.reverse();

    RangesUnion cases;
    cases.ranges(0x0).reserve(s_a); cases.ranges(0x1).reserve(s_b); cases.ranges(0x2).reserve(s_a);
    cases.ranges(0x4).reserve(s_c); cases.ranges(0x5).reserve(s_d); cases.ranges(0x6).reserve(s_c);
    cases.ranges(0x8).reserve(s_c); cases.ranges(0x9).reserve(s_d); cases.ranges(0xA).reserve(s_c);
    cases.ranges(0xC).reserve(s_a); cases.ranges(0xD).reserve(s_b); cases.ranges(0xE).reserve(s_a);

    RANGE_DERIVE(0x0); RANGE_DERIVE(0x1); RANGE_DERIVE(0x2);
    RANGE_DERIVE(0x4); RANGE_DERIVE(0x5); RANGE_DERIVE(0x6);
    RANGE_DERIVE(0x8); RANGE_DERIVE(0x9); RANGE_DERIVE(0xA);
    RANGE_DERIVE(0xC); RANGE_DERIVE(0xD); RANGE_DERIVE(0xE);
    return cases;
}
