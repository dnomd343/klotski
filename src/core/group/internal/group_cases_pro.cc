#include <iostream>

#include <algorithm>

#include "group/group.h"

using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

using klotski::cases::Group;
using klotski::cases::GroupUnion;
using klotski::cases::RangesUnion;
using klotski::cases::GroupCasesPro;

using klotski::cases::ALL_GROUP_NUM;
using klotski::cases::TYPE_ID_LIMIT;
using klotski::cases::ALL_CASES_NUM_;

using klotski::cases::GROUP_DATA;

struct case_info_t {
    uint32_t pattern_id : 10;
    uint32_t toward_id : 2;
    uint32_t case_id  : 20;
};

static_assert(sizeof(case_info_t) == 4);

// TODO: we need multi-thread support (Executor)

static std::vector<std::vector<RangesUnion>> *ru_data = nullptr;
static std::vector<case_info_t> *rev_data = nullptr;

std::vector<std::vector<RangesUnion>> build_ranges_unions() {
    std::vector<std::vector<RangesUnion>> unions;
    unions.reserve(ALL_GROUP_NUM);

    // TODO: add white list for single-group unions

    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto group_union = GroupUnion::unsafe_create(type_id);
        for (uint32_t pattern_id = 0; pattern_id < group_union.pattern_num(); ++pattern_id) {
            std::vector<Group> groups;
            for (auto group : group_union.groups()) {
                if (group.pattern_id() == pattern_id) {
                    groups.emplace_back(group);
                }
            }

            std::vector<RangesUnion> tmp {4};
            for (auto group : groups) {
                tmp[(int)group.toward()] = group.cases();
            }
            unions.emplace_back(tmp);
        }
    }
    return unions;
}

static std::vector<case_info_t> build_tmp_data() {
    std::vector<case_info_t> data;
    data.resize(ALL_CASES_NUM_);
    ShortCode::speed_up(true);

    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto group_union = GroupUnion::unsafe_create(type_id);
        for (auto group : group_union.groups()) {
            uint32_t pattern_id = group.pattern_id();
            auto toward_id = (uint32_t)group.toward();

            // TODO: batch mirror base on pattern
            auto codes = group.cases().codes();
            for (uint32_t case_id = 0; case_id < codes.size(); ++case_id) {
                auto short_code = codes[case_id].to_short_code();
                data[short_code.unwrap()] = case_info_t {
                    .pattern_id = pattern_id,
                    .toward_id = toward_id,
                    .case_id = case_id,
                };
            }
        }
    }
    return data;
}

void GroupCasesPro::build() {
    static auto data_1 = build_ranges_unions();
    static auto data_2 = build_tmp_data();
    ru_data = &data_1;
    rev_data = &data_2;

    // TODO: using std::mutex `busy_`
    fast_ = true;
}

CommonCode GroupCasesPro::fast_obtain_code(CaseInfo info) {

    auto flat_id = PATTERN_OFFSET[info.group.type_id()] + info.group.pattern_id();

    auto &cases = (*ru_data)[flat_id][(int)info.group.toward()];
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

GroupCasesPro::CaseInfo GroupCasesPro::fast_obtain_info(ShortCode short_code) {
    uint16_t type_id = GroupUnion::from_short_code(short_code).unwrap(); // NOTE: need to convert as CommonCode
    uint16_t pattern_id = (*rev_data)[short_code.unwrap()].pattern_id;
    uint16_t toward_id = (*rev_data)[short_code.unwrap()].toward_id;
    auto case_id = (*rev_data)[short_code.unwrap()].case_id;

    return CaseInfo {
        .group = Group::unsafe_create(type_id, pattern_id, (Group::Toward)toward_id),
        .case_id = case_id,
    };
}

GroupCasesPro::CaseInfo GroupCasesPro::fast_obtain_info(CommonCode common_code) {
    auto short_code = common_code.to_short_code();
    uint16_t type_id = GroupUnion::from_common_code(common_code).unwrap();
    uint16_t pattern_id = (*rev_data)[short_code.unwrap()].pattern_id;
    uint16_t toward_id = (*rev_data)[short_code.unwrap()].toward_id;
    auto case_id = (*rev_data)[short_code.unwrap()].case_id;

    return CaseInfo {
        .group = Group::unsafe_create(type_id, pattern_id, (Group::Toward)toward_id),
        .case_id = case_id,
    };
}

CommonCode GroupCasesPro::tiny_obtain_code(CaseInfo info) {
    auto cases = info.group.cases();
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

// NOTE: copy directly from Group impl
static std::unordered_map<uint64_t, Group> build_map_data() {
    // NOTE: using CommonCode as map key
    std::unordered_map<uint64_t, Group> data;
    data.reserve(GROUP_DATA.size());
    for (auto raw : GROUP_DATA) {
        uint32_t type_id = (raw >> 12) & 0b11111111;
        uint32_t pattern_id = (raw >> 2) & 0b1111111111;
        uint32_t toward = raw & 0b11;
        auto seed = CommonCode::unsafe_create(raw >> 20).unwrap();
        auto group = Group::unsafe_create(type_id, pattern_id, (Group::Toward)toward);
        data.emplace(seed, group);
    }
    return data;
}

GroupCasesPro::CaseInfo GroupCasesPro::tiny_obtain_info(CommonCode common_code) {
    auto raw_codes = Group_extend(common_code.to_raw_code());
    std::vector<CommonCode> common_codes;
    common_codes.reserve(raw_codes.size());
    for (auto raw_code : raw_codes) {
        common_codes.emplace_back(raw_code.to_common_code());
    }

    static auto map_data = build_map_data(); // TODO: shared map data with Group::from_raw_code

    auto seed = std::min_element(common_codes.begin(), common_codes.end());
    auto group = map_data.at(seed->unwrap());

    // TODO: try to perf it
    std::sort(common_codes.begin(), common_codes.end());
    auto tmp = std::lower_bound(common_codes.begin(), common_codes.end(), common_code);
    auto case_id = tmp - common_codes.begin();

    return CaseInfo {
        .group = group,
        .case_id = (uint32_t)case_id,
    };
}
