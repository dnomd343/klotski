#include <iostream>

#include "cases.h"

/// Filter cases with different type_id from AllCases.
static std::vector<std::vector<CommonCode>> build_all_cases() {
    std::vector<std::vector<CommonCode>> codes;
    for (auto code : AllCases::instance().fetch().codes()) {
        const auto type_id = to_type_id(cal_block_num(code.unwrap()));
        if (type_id >= codes.size()) {
            codes.resize(type_id + 1);
        }
        codes[type_id].emplace_back(code);
    }
    return codes;
}

std::vector<CommonCode> extend_cases(CommonCode seed) {
    auto raw_codes = klotski::cases::Group::extend(seed.to_raw_code()); // TODO: using inner build process
    std::vector<CommonCode> common_codes {raw_codes.begin(), raw_codes.end()};
    std::ranges::sort(common_codes.begin(), common_codes.end());
    return common_codes;
}

std::vector<CommonCode> remove_sub(const std::vector<CommonCode> &all, const std::vector<CommonCode> &sub) {
    std::vector<CommonCode> tmp;
    tmp.reserve(all.size() - sub.size());
    std::ranges::set_difference(all.begin(), all.end(), sub.begin(), sub.end(), std::back_inserter(tmp));
    return tmp;
}

static std::vector<std::vector<CommonCode>> split_groups(std::vector<CommonCode> codes) {

    std::vector<std::vector<CommonCode>> groups;

    while (!codes.empty()) {
        auto sub = extend_cases(codes[0]);

        groups.emplace_back(sub);
        codes = remove_sub(codes, sub);
    }

    std::ranges::stable_sort(groups.begin(), groups.end(), [](const std::vector<CommonCode> &g1, const std::vector<CommonCode> &g2) {
        return g1.size() > g2.size();
    });

    return groups;

}

// TODO: static data of `build_all_cases`

uint32_t group_union_num() {
    static auto data = build_all_cases();
    return data.size();
}

const std::vector<CommonCode>& group_union_cases(const uint32_t type_id) {
    static auto data = build_all_cases();
    if (type_id < data.size()) {
        return data[type_id];
    }
    std::abort();
}

// TODO: multi-threads builder

static std::vector<std::vector<std::vector<CommonCode>>> all_groups_builder() {

    std::vector<std::vector<std::vector<CommonCode>>> data;

    for (uint32_t type_id = 0; type_id < group_union_num(); ++type_id) {

        data.emplace_back(split_groups(group_union_cases(type_id)));

    }

    return data;

}

uint32_t group_num(uint32_t type_id) {
    static auto data = all_groups_builder();
    if (type_id < data.size()) {
        return data[type_id].size();
    }
    std::abort();
}

const std::vector<CommonCode>& group_cases(uint32_t type_id, uint32_t group_id) {

    static auto data = all_groups_builder();

    if (type_id < data.size() && group_id < data[type_id].size()) {
        return data[type_id][group_id];
    }

    std::abort();
}
