#include <queue>
#include <iostream>

//#include <map>
//#include <unordered_map>
//#include <boost/unordered_map.hpp>
//#include <ankerl/unordered_dense.h>
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

//struct custom_hash_simple {
//
//    using is_avalanching = void;
//
//    auto operator()(uint64_t const& x) const noexcept -> uint64_t {
//        return ankerl::unordered_dense::detail::wyhash::hash(static_cast<uint64_t>(x));
//    }
//};

uint32_t Group::demo(const RawCode &seed) {
    struct group_cal_t {
        uint64_t code;
        uint64_t mask;
    };

    std::queue<group_cal_t*> cache;

//    std::map<uint64_t, group_cal_t> cases;
//    std::unordered_map<uint64_t, group_cal_t> cases;
//    boost::unordered_map<uint64_t, group_cal_t> cases;
//    ankerl::unordered_dense::map<uint64_t, group_cal_t> cases;
    absl::flat_hash_map<uint64_t, group_cal_t> cases;

    cases.reserve(65535 * 8);

    cache.emplace(&cases.emplace(seed.unwrap(), group_cal_t {
        .code = seed.unwrap(),
        .mask = 0,
    }).first->second);

    auto core = Core(
        [&cases, &cache](auto &&code, auto &&mask) {
            auto current = cases.find(code);

            if (current != cases.end()) { // find existed case
                current->second.mask |= mask; // update mask info
                return;
            }

            cache.emplace(&cases.emplace(code, group_cal_t {
                .code = code,
                .mask = mask,
            }).first->second);
        }
    );

    while (!cache.empty()) {
        core.next_cases(cache.front()->code, cache.front()->mask);
        cache.pop();
    }

//    std::cout << cases.size() << std::endl;

    return cases.size();
}


} // namespace klotski
