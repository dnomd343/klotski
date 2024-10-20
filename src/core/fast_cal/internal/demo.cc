#include "fast_cal/fast_cal.h"

#include <iostream>

#include <queue>

#include <absl/container/flat_hash_map.h>

#include "mover/mover.h"
#include "group/group.h"

using klotski::codec::RawCode;
using klotski::codec::CommonCode;
using klotski::cases::RangesUnion;

using klotski::mover::MaskMover;
using klotski::cases::GroupUnion;

struct data_t {
    uint64_t mask;
    uint64_t back;
};

// TODO: try using `std::vector` + offset instead of `std::queue`

RawCode FastCal_demo(RawCode raw_code) {
    std::queue<RawCode> cache;
    absl::flat_hash_map<uint64_t, data_t> cases; // <code, mask>
    auto reserve = GroupUnion::from_raw_code(raw_code).max_group_size();
    cases.reserve(reserve);

    auto core = MaskMover([&cache, &cases](uint64_t code, uint64_t mask) {
        if (const auto match = cases.find(code); match != cases.end()) {
            match->second.mask |= mask; // update mask
            return;
        }
        cases.emplace(code, data_t {
            .mask = mask,
            .back = cache.front().unwrap(),
        });
        cache.emplace(RawCode::unsafe_create(code));
    });

    cache.emplace(raw_code);
    cases.emplace(raw_code, data_t {0, 0}); // without mask

    while (!cache.empty()) {
        if (((cache.front().unwrap() >> 39) & 0b111) == 0b100) {
            break;
        }
        uint64_t curr = cache.front().unwrap();
        core.next_cases(curr, cases.find(curr)->second.mask);
        cache.pop();
    }

//    std::vector<RawCode> path;
//    auto code = cache.front().unwrap();
//    while (true) {
//        if (code == 0) {
//            break;
//        }
//        path.emplace_back(RawCode::unsafe_create(code));
//        code = cases.find(code)->second.back;
//    }
//    std::reverse(path.begin(), path.end());
//    for (auto step : path) {
//        std::cout << step << std::endl;
//    }
//    std::cout << path.size() << std::endl;

    return cache.front();

}
