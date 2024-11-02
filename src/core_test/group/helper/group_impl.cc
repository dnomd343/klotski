#include <algorithm>
#include <mover/mover.h>

#include "group/group.h"
#include "helper/block_num.h"

// TODO: multi-threads builder

using klotski::codec::RawCode;
using klotski::cases::AllCases;
using klotski::codec::CommonCode;

using klotski::mover::MaskMover;

uint32_t group_union_num();

const std::vector<CommonCode>& group_union_cases(uint32_t type_id);

static std::vector<RawCode> Group_extend(RawCode raw_code) {
    std::vector<RawCode> codes;
    std::unordered_map<uint64_t, uint64_t> cases; // <code, mask>

    auto core = MaskMover([&codes, &cases](RawCode code, uint64_t mask) {
        if (const auto match = cases.find(code.unwrap()); match != cases.end()) {
            match->second |= mask; // update mask
            return;
        }
        cases.emplace(code, mask);
        codes.emplace_back(code); // new case
    });

    uint64_t offset = 0;
    codes.emplace_back(raw_code);
    cases.emplace(raw_code, 0); // without mask
    while (offset != codes.size()) {
        auto curr = codes[offset++].unwrap();
        core.next_cases(RawCode::unsafe_create(curr), cases.find(curr)->second);
    }
    return codes;
}

/// Extend ordered Group from the specified CommonCode seed.
static std::vector<CommonCode> extend_cases(CommonCode seed) {
    // TODO: using inner build process -> only allow calling klotski::mover
    auto raw_codes = Group_extend(seed.to_raw_code());
    std::vector<CommonCode> common_codes {raw_codes.begin(), raw_codes.end()};
    std::ranges::sort(common_codes.begin(), common_codes.end());
    return common_codes;
}

/// Split Groups from the specified ordered list of CommonCodes.
static std::vector<std::vector<CommonCode>> split_groups(std::vector<CommonCode> codes) {
    std::vector<std::vector<CommonCode>> groups;
    while (!codes.empty()) {
        auto group = extend_cases(codes[0]); // sorted array
        groups.emplace_back(group);

        std::vector<CommonCode> remain;
        remain.reserve(codes.size() - group.size());
        std::ranges::set_difference(codes.begin(), codes.end(), group.begin(), group.end(), std::back_inserter(remain));
        codes = remain; // for next loop
    }
    std::ranges::stable_sort(groups.begin(), groups.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.size() > rhs.size(); // sort with group size
    });
    return groups;
}

/// Filter cases with different type_id from AllCases.
static const std::vector<std::vector<CommonCode>>& group_union_data() {
    static auto data = [] {
        std::vector<std::vector<CommonCode>> codes;
        codes.resize(helper::block_nums().size());
        for (auto code: AllCases::instance().fetch().codes()) {
            codes[to_type_id(helper::cal_block_num(code.unwrap()))].emplace_back(code);
        }
        return codes;
    }();
    return data;
}

/// Construct all valid klotski Groups in different GroupUnion.
static const std::vector<std::vector<std::vector<CommonCode>>>& group_data() {
    static auto data = [] {
        std::vector<std::vector<std::vector<CommonCode>>> groups;
        groups.reserve(group_union_num());
        for (uint32_t type_id = 0; type_id < group_union_num(); ++type_id) {
            groups.emplace_back(split_groups(group_union_cases(type_id)));
        }
        return groups;
    }();
    return data;
}

uint32_t group_union_num() {
    return group_union_data().size();
}

uint32_t group_num(uint32_t type_id) {
    if (type_id < group_data().size()) {
        return group_data()[type_id].size();
    }
    std::abort();
}

const std::vector<CommonCode>& group_union_cases(const uint32_t type_id) {
    if (type_id < group_union_data().size()) {
        return group_union_data()[type_id];
    }
    std::abort();
}

const std::vector<CommonCode>& group_cases(uint32_t type_id, uint32_t group_id) {
    if (type_id < group_data().size() && group_id < group_data()[type_id].size()) {
        return group_data()[type_id][group_id];
    }
    std::abort();
}
