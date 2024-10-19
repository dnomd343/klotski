#include <iostream>
#include <algorithm>

#include "group/group.h"

using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

//using klotski::cases::GroupCases;
using klotski::cases::GroupUnion;
using klotski::cases::RangesUnion;

using klotski::cases::ALL_GROUP_NUM;
using klotski::cases::TYPE_ID_LIMIT;
using klotski::cases::ALL_CASES_NUM_;

struct tmp_t {
    uint32_t group_id : 12;
    uint32_t case_id  : 20;
};

static_assert(sizeof(tmp_t) == 4);

// TODO: we need multi-thread support (Executor)

static std::vector<tmp_t> *rev_data = nullptr;
static std::vector<RangesUnion> *ru_data = nullptr;

static std::vector<RangesUnion> build_ranges_unions() {
    std::vector<RangesUnion> unions;
    unions.reserve(ALL_GROUP_NUM);

    // TODO: add white list for single-group unions

    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto group_union = GroupUnion::unsafe_create(type_id);
//        for (auto group : group_union.groups()) {
//            unions.emplace_back(group.cases());
//        }
    }
    return unions;
}

static std::vector<tmp_t> build_tmp_data() {
    std::vector<tmp_t> data;
    data.resize(ALL_CASES_NUM_);
    ShortCode::speed_up(true);

    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto group_union = GroupUnion::unsafe_create(type_id);
//        for (auto group : group_union.groups()) {
//            uint32_t group_id = group.group_id();
//
//            auto codes = group.cases().codes();
//            for (uint32_t case_id = 0; case_id < codes.size(); ++case_id) {
//                auto short_code = codes[case_id].to_short_code();
//
//                data[short_code.unwrap()] = tmp_t {
//                    .group_id = group_id,
//                    .case_id = case_id,
//                };
//            }
//        }
    }
    return data;
}

//void GroupCases::build() {
//    static auto data_1 = build_ranges_unions();
//    static auto data_2 = build_tmp_data();
//    ru_data = &data_1;
//    rev_data = &data_2;
//}

//CommonCode GroupCases::fast_parse(Info info) {
//
//    auto flat_id = GROUP_OFFSET[info.type_id] + info.group_id;
//
//    auto &cases = (*ru_data)[flat_id];
//    // TODO: make offset table for perf
//
//    uint64_t head = 0;
//
//    for (;;) {
//        if (info.case_id >= cases[head].size()) {
//            info.case_id -= cases[head].size();
//            ++head;
//        } else {
//            break;
//        }
//    }
//
//    auto range = cases[head][info.case_id];
//    return CommonCode::unsafe_create(head << 32 | range);
//}

//GroupCases::Info GroupCases::fast_obtain(codec::ShortCode short_code) {
//
//    uint16_t type_id = GroupUnion::from_short_code(short_code).unwrap(); // NOTE: need to convert as CommonCode
//    uint16_t group_id = (*rev_data)[short_code.unwrap()].group_id;
//    auto case_id = (*rev_data)[short_code.unwrap()].case_id;
//
//    return Info {
//        .type_id = type_id,
//        .group_id = group_id,
//        .case_id = case_id,
//    };
//}

//GroupCases::Info GroupCases::fast_obtain(codec::CommonCode common_code) {
//    return fast_obtain(common_code.to_short_code());
//}
//
//void GroupCases::build_async(klotski::Executor &&executor, klotski::Notifier &&callback) {
//
//}
//
//klotski::codec::CommonCode GroupCases::tiny_parse(klotski::cases::GroupCases::Info info) {
//    // TODO: tiny parse process
//    return CommonCode::unsafe_create(0);
//}
//
//GroupCases::Info GroupCases::tiny_obtain(codec::CommonCode common_code) {
//    // TODO: tiny obtain process
//    return Info {};
//}
