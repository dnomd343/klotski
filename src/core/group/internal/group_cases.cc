#include <iostream>

#include "group/group.h"

using klotski::cases::GroupCases;

using klotski::codec::CommonCode;

using klotski::cases::RangesUnion;

struct tmp_t {
    uint32_t group_id : 12;
    uint32_t case_id  : 20;
};

static_assert(sizeof(tmp_t) == 4);

// TODO: we need multi-thread support (Executor)

std::vector<RangesUnion> build_ranges_unions() {

    std::vector<RangesUnion> unions;
    unions.reserve(25422);

    // auto group_union = klotski::cases::GroupUnion::unsafe_create(169);

    // TODO: add white list for single-group unions

    for (uint32_t type_id = 0; type_id < klotski::cases::TYPE_ID_LIMIT; ++type_id) {
        auto group_union = klotski::cases::GroupUnion::unsafe_create(type_id);
        for (auto group : group_union.groups()) {
            unions.emplace_back(group.cases());
        }
    }

    std::cout << unions.size() << std::endl;

    return unions;
}

CommonCode GroupCases::from_info_t(info_t info) {

    std::vector<RangesUnion> data = build_ranges_unions();

    // TODO: build as static data

    // TODO: loop [0, 25422) -> get RangesUnion of all flat_ids

    auto flat_id = GROUP_OFFSET[info.type_id] + info.group_id;
    std::cout << "flat_id = " << flat_id << std::endl;

    // auto codes = data[flat_id].codes();
    // std::stable_sort(codes.begin(), codes.end());
    // std::cout << codes[info.case_id] << std::endl;

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

    std::cout << "head = " << head << std::endl;
    std::cout << info.case_id << std::endl;

    // TODO: need sort in `Group::cases`
    std::stable_sort(cases[head].begin(), cases[head].end());

    std::cout << CommonCode::unsafe_create(head << 32 | cases[1][1909]) << std::endl;

    return CommonCode::unsafe_create(0);
}

GroupCases::info_t GroupCases::to_info_t(codec::ShortCode short_code) {
    std::vector<tmp_t> data;

    // TODO: build as static data

    // TODO: loop from `std::vector<RangesUnion>`

    return {};
}
