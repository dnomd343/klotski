// #include <absl/container/flat_hash_map.h>

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
    phmap::flat_hash_map<uint64_t, uint64_t> cases; // <code, mask>
//    reserve = reserve ? reserve : GroupUnion::from_raw_code(raw_code).max_group_size();
//    reserve = 25955;
    codes.reserve(GroupUnion::from_raw_code(raw_code).max_group_size());
    cases.reserve(25955 * 1.56);

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
//    std::cout << cases.size() << std::endl;
//    std::cout << cases.load_factor() << std::endl;
    return codes;
}
