#include <queue>
#include <iostream>

#include "absl/container/flat_hash_map.h"

#include "group.h"
#include "common.h"
#include "core.h"

namespace klotski {

using Common::range_reverse;

Group::block_num_t Group::block_num(const RawCode &raw_code) {
    block_num_t result;
    auto tmp = raw_code.unwrap();
    for (int addr = 0; addr < 20; ++addr, tmp >>= 3) {
        switch (tmp & 0b111) {
            case B_1x1:
                ++result.n_1x1;
                continue;
            case B_1x2:
                ++result.n_1x2;
                continue;
            case B_2x1:
                ++result.n_2x1;
                continue;
        }
    }
    return result;
}

Group::block_num_t Group::block_num(const CommonCode &common_code) {
    block_num_t result;
    auto range = range_reverse((uint32_t)common_code.unwrap());
    for (; range; range >>= 2) {
        switch (range & 0b11) {
            case 0b01: /// 1x2 block
                ++result.n_1x2;
                continue;
            case 0b10: /// 2x1 block
                ++result.n_2x1;
                continue;
            case 0b11: /// 1x1 block
                ++result.n_1x1;
                continue;
        }
    }
    return result;
}

uint32_t Group::demo(const RawCode &seed) {

//    struct group_cal_t {
//        uint64_t code;
//        uint64_t mask;
//    };
//    std::queue<group_cal_t*> cache;
//    std::queue<uint64_t> cache;
//    std::queue<std::pair<uint64_t, uint64_t>> cache;
//
//    absl::flat_hash_map<uint64_t, group_cal_t> cases;
//    absl::flat_hash_map<uint64_t, uint64_t> cases;
//    absl::flat_hash_map<uint64_t, std::unique_ptr<group_cal_t>> cases;

    std::queue<uint64_t> cache;
    std::queue<uint64_t> cache_;
    absl::flat_hash_map<uint64_t, uint64_t> cases;

    cases.reserve(65535 * 8);

//    cache.emplace(&cases.emplace(seed.unwrap(), group_cal_t {
//        .code = seed.unwrap(),
//        .mask = 0,
//    }).first->second);
//    cases.emplace(seed.unwrap(), 0);
//    cache.emplace(seed.unwrap());
//    cache.emplace(seed.unwrap(), 0);
//    cases.emplace(seed.unwrap(), std::make_unique<group_cal_t>(group_cal_t {
//        .code = seed.unwrap(),
//        .mask = 0,
//    }));

    cases.emplace(seed.unwrap(), 0);
    cache.emplace(seed.unwrap());
    cache_.emplace(0);

    auto core = Core(
        [&cases, &cache, &cache_](auto &&code, auto &&mask) {
            auto current = cases.find(code);

            if (current != cases.end()) { // find existed case

//                current->second.mask |= mask; // update mask info
//                current->second |= mask;
//                current->second->mask |= mask;

                current->second |= mask; // update mask info
                return;
            }

//            cache.emplace(&cases.emplace(code, group_cal_t {
//                .code = code,
//                .mask = mask,
//            }).first->second);
//            cases.emplace(code, mask);
//                cache.emplace(code);
//                cache.emplace(code, mask);
//            cases.emplace(code, std::make_unique<group_cal_t>(group_cal_t {
//                .code = code,
//                .mask = mask,
//            }));

            cases.emplace(code, mask);
            cache.emplace(code);
            cache_.emplace(mask);

        }
    );

    while (!cache.empty()) {
//        core.next_cases(cache.front()->code, cache.front()->mask);
//        core.next_cases(cache.front(), cases.find(cache.front())->second);
//        core.next_cases(cache.front().first, cache.front().second);
//        core.next_cases(cache.front(), cases.find(cache.front())->second->mask);
//        core.next_cases(cache.front(), cases.find(cache.front())->second);
        core.next_cases(cache.front(), cache_.front());

        cache.pop();
        cache_.pop();
    }

//    std::cout << cases.size() << std::endl;

    return cases.size();
}


} // namespace klotski
