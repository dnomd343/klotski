#include <queue>
#include "core.h"
#include "group.h"
#include "common.h"
#include "absl/container/flat_hash_map.h"

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
    std::queue<uint64_t> cache;

//    uint32_t index = 0;
//    std::vector<uint64_t> temp;
//    temp.reserve(65535 * 8);

    absl::flat_hash_map<uint64_t, uint64_t> cases;

    cases.reserve(65535 * 8);

    cases.emplace(seed.unwrap(), 0); // without mask
    cache.emplace(seed.unwrap());

//    temp.emplace_back(seed.unwrap());

    auto core = Core(
        [&cases, &cache](auto &&code, auto &&mask) {
            auto current = cases.find(code);

            if (current != cases.end()) {
                current->second |= mask; // update mask info
                return;
            }

            cases.emplace(code, mask);
            cache.emplace(code);
//            temp.emplace_back(code);

        }
    );

    while (!cache.empty()) {
//    while (index != temp.size()) {

        core.next_cases(cache.front(), cases.find(cache.front())->second);
//        core.next_cases(temp[index], cases.find(temp[index])->second);

        cache.pop();
//        ++index;

    }

    return cases.size();
}


} // namespace klotski
