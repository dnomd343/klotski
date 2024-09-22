#include <iostream>
#include <algorithm>

#include "group/group.h"

using klotski::cases::GroupCases;

using klotski::codec::ShortCode;

using klotski::codec::CommonCode;

using klotski::cases::RangesUnion;

using klotski::cases::GroupUnion;

using klotski::cases::ALL_GROUP_NUM;

using klotski::cases::TYPE_ID_LIMIT;

using klotski::cases::ALL_CASES_NUM_;

struct tmp_t {
    uint32_t group_id : 12;
    uint32_t case_id  : 20;
};

static_assert(sizeof(tmp_t) == 4);

// TODO: we need multi-thread support (Executor)

std::vector<RangesUnion> build_ranges_unions() {

    std::vector<RangesUnion> unions;
    unions.reserve(ALL_GROUP_NUM);

    // TODO: add white list for single-group unions

    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto group_union = GroupUnion::unsafe_create(type_id);
        for (auto group : group_union.groups()) {
            unions.emplace_back(group.cases());
        }
    }

    return unions;
}

CommonCode GroupCases::from_info_t(info_t info) {

    // TODO: build as static class member (ptr)
    static std::vector<RangesUnion> data = build_ranges_unions();

    auto flat_id = GROUP_OFFSET[info.type_id] + info.group_id;

    auto &cases = data[flat_id];
    // TODO: make offset table for perf

    uint64_t head = 0;

    for (;;) {
        if (info.case_id >= cases[head].size()) {
            info.case_id -= cases[head].size();
            ++head;
        } else {
            break;
        }
    }

    auto range = cases[head][info.case_id];
    return CommonCode::unsafe_create(head << 32 | range);
}

static std::vector<tmp_t> build_tmp_data() {

    std::vector<tmp_t> data;
    data.resize(ALL_CASES_NUM_);
    ShortCode::speed_up(true);

    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto group_union = GroupUnion::unsafe_create(type_id);
        for (auto group : group_union.groups()) {
            uint32_t group_id = group.group_id();

            auto codes = group.cases().codes();
            for (uint32_t case_id = 0; case_id < codes.size(); ++case_id) {
                auto short_code = codes[case_id].to_short_code();

                data[short_code.unwrap()] = tmp_t {
                    .group_id = group_id,
                    .case_id = case_id,
                };

            }
        }
    }

    return data;

}

GroupCases::info_t GroupCases::to_info_t(codec::ShortCode short_code) {

    // TODO: build as static class member (ptr)
    static auto data = build_tmp_data();

    uint16_t type_id = GroupUnion::from_short_code(short_code).unwrap(); // NOTE: need to convert as CommonCode
    uint16_t group_id = data[short_code.unwrap()].group_id;
    auto case_id = data[short_code.unwrap()].case_id;

    return info_t {
        .type_id = type_id,
        .group_id = group_id,
        .case_id = case_id,
    };
}
