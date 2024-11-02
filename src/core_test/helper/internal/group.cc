#include "helper/group.h"
#include "helper/block_num.h"

#include "group/group.h"
#include "all_cases/all_cases.h"

#include <iostream>
#include <algorithm>

#include <unordered_set>
#include <mover/mover.h>

using klotski::cases::AllCases;

using klotski::codec::RawCode;
using klotski::codec::CommonCode;

using klotski::mover::MaskMover;

#define STATIC_DATA(name, impl)         \
    static const auto& name() {         \
        static auto data = [] {impl}(); \
        return data;                    \
    }



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







/// Filter cases with different type_id from AllCases.
STATIC_DATA(group_union_data, {
    std::vector<std::vector<CommonCode>> codes;
    codes.resize(helper::block_nums().size());
    for (const auto code: AllCases::instance().fetch().codes()) {
        const auto block_num = helper::cal_block_num(code.unwrap());
        codes[to_type_id(block_num)].emplace_back(code);
    }
    return codes;
})

uint32_t helper::group_union_num() {
    return group_union_data().size();
}

const std::vector<CommonCode>& helper::group_union_cases(const uint32_t type_id) {
    if (type_id >= group_union_data().size()) {
        std::abort();
    }
    return group_union_data()[type_id];
}










/// Extend ordered Group from the specified CommonCode seed.
static std::vector<CommonCode> extend_cases(CommonCode seed) {
    // TODO: using inner build process -> only allow calling klotski::mover
    auto raw_codes = Group_extend(seed.to_raw_code());
    std::vector<CommonCode> common_codes {raw_codes.begin(), raw_codes.end()};
    std::ranges::sort(common_codes.begin(), common_codes.end());
    return common_codes;
}

struct Pattern {
    enum class Mirror {
        Full = 0,
        V = 1,
        HV = 2,
        H = 3,
        Common = 4,
    };

    Pattern(CommonCode s) : seed(s) {}

    Mirror mirror;
    uint32_t size;
    CommonCode seed;
    uint32_t group_size;
    std::array<std::vector<CommonCode>, 4> cases {};
};

static std::vector<CommonCode> split_group(std::unordered_set<uint64_t> &codes, CommonCode origin) {
    std::vector<CommonCode> group;
    for (auto code : extend_cases(origin)) {
        codes.erase(code.unwrap());
        group.emplace_back(code);
    }
    std::sort(group.begin(), group.end());
    return group;
}

static std::vector<Pattern> split_patterns(const std::vector<CommonCode> &common_codes) {
    std::unordered_set<uint64_t> codes;
    for (auto code : common_codes) {
        codes.emplace(code.unwrap());
    }

    std::vector<Pattern> patterns;
    while (true) {
        if (codes.empty()) {
            break;
        }

        auto code_a = CommonCode::unsafe_create(*std::min_element(codes.begin(), codes.end()));
        auto code_b = code_a.to_horizontal_mirror();
        auto code_c = code_a.to_vertical_mirror();
        auto code_d = code_b.to_vertical_mirror();

        auto group_a = split_group(codes, code_a);

        Pattern pattern {code_a};
        pattern.cases[0] = group_a;
        pattern.size = group_a.size();
        pattern.group_size = group_a.size();

        if (!codes.contains(code_b.unwrap()) && !codes.contains(code_c.unwrap()) && !codes.contains(code_d.unwrap())) {
            pattern.mirror = Pattern::Mirror::Full;
        } else if (codes.contains(code_b.unwrap()) && codes.contains(code_c.unwrap()) && codes.contains(code_d.unwrap())) {
            pattern.mirror = Pattern::Mirror::Common;
            pattern.cases[1] = split_group(codes, code_b);
            pattern.cases[2] = split_group(codes, code_c);
            pattern.cases[3] = split_group(codes, code_d);
            pattern.size *= 4;
        } else if (!codes.contains(code_b.unwrap())) {
            pattern.mirror = Pattern::Mirror::V;
            pattern.cases[1] = split_group(codes, code_c);
            pattern.size *= 2;
        } else if (!codes.contains(code_c.unwrap())) {
            pattern.mirror = Pattern::Mirror::H;
            pattern.cases[1] = split_group(codes, code_b);
            pattern.size *= 2;
        } else if (!codes.contains(code_d.unwrap())) {
            pattern.mirror = Pattern::Mirror::HV;
            pattern.cases[1] = split_group(codes, code_b);
            pattern.size *= 2;
        } else {
            std::cout << "unknown pattern" << std::endl;
            break;
        }

        patterns.emplace_back(pattern);
    }

    std::sort(patterns.begin(), patterns.end(), [](const Pattern &lhs, const Pattern &rhs) {
        if (lhs.size > rhs.size) {
            return true;
        }
        if (lhs.size < rhs.size) {
            return false;
        }

        if ((int)lhs.mirror < (int)rhs.mirror) {
            return true;
        }
        if ((int)lhs.mirror > (int)rhs.mirror) {
            return false;
        }

        if (lhs.seed < rhs.seed) {
            return true;
        }
        if (lhs.seed > rhs.seed) {
            return false;
        }

        std::cout << "pattern compare error" << std::endl;
        return false;
    });
    return patterns;
}

STATIC_DATA(pattern_data, {
    std::vector<std::vector<Pattern>> patterns;
    for (const auto &group_union : group_union_data()) {
        patterns.emplace_back(split_patterns(group_union));
    }
    return patterns;
})

uint32_t helper::group_union_pattern_num(uint32_t type_id) {
    if (type_id >= group_union_data().size()) {
        std::abort();
    }
    return pattern_data()[type_id].size();
}

uint32_t helper::group_union_group_num(uint32_t type_id) {
    if (type_id >= group_union_data().size()) {
        std::abort();
    }

    uint32_t group_num {0};
    for (const auto &pattern : pattern_data()[type_id]) {
        switch (pattern.mirror) {
            case Pattern::Mirror::Full:
                ++group_num;
                break;
            case Pattern::Mirror::V:
            case Pattern::Mirror::HV:
            case Pattern::Mirror::H:
                group_num += 2;
                break;
            case Pattern::Mirror::Common:
                group_num += 4;
                break;
        }
    }
    return group_num;
}

uint8_t helper::pattern_mirror_type(uint32_t type_id, uint32_t pattern_id) {
    if (type_id >= group_union_data().size() || pattern_id >= pattern_data()[type_id].size()) {
        std::abort();
    }

    const auto &pattern = pattern_data()[type_id][pattern_id];
    return (uint8_t)pattern.mirror;
}

std::vector<uint8_t> helper::pattern_toward_list(uint32_t type_id, uint32_t pattern_id) {
    if (type_id >= group_union_data().size() || pattern_id >= pattern_data()[type_id].size()) {
        std::abort();
    }

    const auto &pattern = pattern_data()[type_id][pattern_id];
    switch (pattern.mirror) {
        case Pattern::Mirror::Full:
            return {0};
        case Pattern::Mirror::V:
            return {0, 2};
        case Pattern::Mirror::HV:
        case Pattern::Mirror::H:
            return {0, 1};
        case Pattern::Mirror::Common:
            return {0, 1, 2, 3};
    }
}

const std::vector<CommonCode> &helper::group_cases(uint32_t type_id, uint32_t pattern_id, uint8_t toward) {
    if (type_id >= group_union_data().size() || pattern_id >= pattern_data()[type_id].size()) {
        std::abort();
    }

    const auto &pattern = pattern_data()[type_id][pattern_id];
    if (toward == 0) {
        return pattern.cases[0];
    } else if (toward == 1) {
        switch (pattern.mirror) {
            case Pattern::Mirror::Full:
            case Pattern::Mirror::V:
                std::abort();
            case Pattern::Mirror::HV:
            case Pattern::Mirror::H:
            case Pattern::Mirror::Common:
                return pattern.cases[1];
        }
    } else if (toward == 2) {
        switch (pattern.mirror) {
            case Pattern::Mirror::Full:
            case Pattern::Mirror::HV:
            case Pattern::Mirror::H:
                std::abort();
            case Pattern::Mirror::V:
                return pattern.cases[1];
            case Pattern::Mirror::Common:
                return pattern.cases[2];
        }
    } else if (toward == 3) {
        switch (pattern.mirror) {
            case Pattern::Mirror::Full:
            case Pattern::Mirror::V:
            case Pattern::Mirror::HV:
            case Pattern::Mirror::H:
                std::abort();
            case Pattern::Mirror::Common:
                return pattern.cases[3];
        }
    } else {
        std::abort();
    }
}
