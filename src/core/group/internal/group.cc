#include <absl/container/flat_hash_map.h>

#include "core/core.h"
#include "group/group.h"

using klotski::core::Core;
using klotski::cases::Group;
using klotski::codec::RawCode;
using klotski::codec::CommonCode;
using klotski::cases::RangesUnion;

std::vector<RawCode> Group::extend(RawCode raw_code, uint32_t reserve) {
    std::vector<RawCode> codes;
    absl::flat_hash_map<uint64_t, uint64_t> cases; // <code, mask>
    reserve = reserve ? reserve : GroupUnion::from_raw_code(raw_code).max_group_size();
    codes.reserve(reserve);
    cases.reserve(reserve);

    auto core = Core([&codes, &cases](uint64_t code, uint64_t mask) {
        if (const auto match = cases.find(code); match != cases.end()) {
            match->second |= mask; // update mask
            return;
        }
        cases.emplace(code, mask);
        codes.emplace_back(RawCode::unsafe_create(code)); // new case
    });

    uint64_t offset = 0;
    codes.emplace_back(raw_code);
    cases.emplace(raw_code, 0); // without mask
    while (offset != codes.size()) {
        auto curr = codes[offset++].unwrap();
        core.next_cases(curr, cases.find(curr)->second);
    }
    return codes;
}

RangesUnion Group::cases() const {

    // TODO: add white list for single-group unions

    auto seed = CommonCode::unsafe_create(GROUP_SEED[flat_id()]);

    // std::cout << seed << std::endl;

    auto codes = extend(seed.to_raw_code(), size());

    // std::cout << codes.size() << std::endl;

    // TODO: how to reserve

    RangesUnion data;

    for (auto raw_code : codes) {
        auto common_code = raw_code.to_common_code().unwrap();
        data[common_code >> 32].emplace_back(static_cast<uint32_t>(common_code));
    }

    // TODO: do sort process

    return data;
}
