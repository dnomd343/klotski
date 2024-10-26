#include <absl/container/flat_hash_map.h>

#include <parallel_hashmap/phmap.h>

#include "mover/mover.h"
#include "group/group.h"

//using klotski::cases::Group;
using klotski::codec::RawCode;
using klotski::codec::CommonCode;
using klotski::cases::RangesUnion;

using klotski::mover::MaskMover;
using klotski::cases::GroupUnion;

// TODO: maybe we can perf with mirror cases

std::vector<RawCode> klotski::cases::Group_extend(RawCode raw_code, uint32_t reserve) {
    std::vector<RawCode> codes;
//    absl::flat_hash_map<uint64_t, uint64_t> cases; // <code, mask>
    phmap::flat_hash_map<uint64_t, uint64_t> cases; // <code, mask>
//    reserve = reserve ? reserve : GroupUnion::from_raw_code(raw_code).max_group_size();
//    reserve = 25955;
    codes.reserve(GroupUnion::from_raw_code(raw_code).max_group_size());
    cases.reserve(25955 * 1.56);

    auto core = MaskMover([&codes, &cases](uint64_t code, uint64_t mask) {
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
//    std::cout << cases.size() << std::endl;
//    std::cout << cases.load_factor() << std::endl;
    return codes;
}

double klotski::cases::Group_load_factor(RawCode raw_code, double coff) {
    std::vector<RawCode> codes;
    phmap::flat_hash_map<uint64_t, uint64_t> cases; // <code, mask>
    const auto reserve = GroupUnion::from_raw_code(raw_code).max_group_size();
    codes.reserve(reserve);
    cases.reserve(static_cast<size_t>(coff * reserve));

    auto core = MaskMover([&codes, &cases](uint64_t code, uint64_t mask) {
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

//    if (cases.size() != reserve) {
//        std::cout << "reserve size error" << std::endl;
//        std::abort();
//    }
    return cases.load_factor();
}

//RangesUnion Group::cases() const {
//
//    // TODO: add white list for single-group unions
//    //       return GroupUnion::cases directly
//
//    auto seed = CommonCode::unsafe_create(GROUP_SEED[flat_id()]);
//
//    // std::cout << seed << std::endl;
//
//    auto codes = Group_extend(seed.to_raw_code(), size());
//
//    // std::cout << codes.size() << std::endl;
//
//    // TODO: how to reserve
//
//    RangesUnion data;
//
//    for (auto raw_code : codes) {
//        auto common_code = raw_code.to_common_code().unwrap();
//        data[common_code >> 32].emplace_back(static_cast<uint32_t>(common_code));
//    }
//
//    // TODO: do sort process
//
//    for (int head = 0; head < 16; ++head) {
//        std::stable_sort(data[head].begin(), data[head].end());
//    }
//
//    return data;
//}

//Group Group::from_raw_code(codec::RawCode raw_code) {
//
//    auto raw_codes = Group_extend(raw_code);
//
//    auto common_codes = raw_codes | std::views::transform([](const RawCode r) {
//        return r.to_common_code();
//    }) | std::ranges::to<std::vector>(); // TODO: search min_element directly
//
//    auto seed = std::min_element(common_codes.begin(), common_codes.end());
//
//    std::cout << *seed << std::endl;
//
//    // TODO: search type_id / group_id from map
//    auto flat_id = std::find(GROUP_SEED.begin(), GROUP_SEED.end(), *seed) - GROUP_SEED.begin();
//    std::cout << flat_id << std::endl;
//
//    return Group::unsafe_create(0, 0); // TODO: only for compile
//}
