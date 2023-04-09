#include <queue>
#include <vector>
#include "core.h"
#include "group.h"
#include "common.h"
#include "absl/container/flat_hash_map.h"

#include "type_id.h"

namespace klotski {

using Common::range_reverse;

Group::block_num_t Group::block_num(uint32_t type_id) {

    // jiang_num (n_x2x)
    // bing_num  (n_1x1)
    // style_num (n_2x1)

    auto tids = std::vector<uint32_t>();

    uint32_t count = 0;
    for (int n_x2x = 0; n_x2x <= 7; ++n_x2x)
        for (int n_2x1 = 0; n_2x1 <= n_x2x; ++n_2x1)
            for (int n_1x1 = 0; n_1x1 <= (14 - n_x2x * 2); ++n_1x1) {
                ++count;

                uint32_t tid = (n_x2x << 8) | (n_2x1 << 4) | n_1x1;
                tids.emplace_back(tid);

                std::cout << n_x2x << " " << n_1x1 << " " << n_2x1 << "(" << tid << ")" << std::endl;
            }

//    std::cout << count << std::endl;

//    for (int i = 0; i < tids.size(); ++i) {
//
//        printf("% 5d,", tids[i]);
//
//        if ((i & 0b111) == 0b111) {
//            printf("\n");
//        }

//        std::cout << tid << std::endl;
//    }


//                generate(generate_t { // generate target ranges
//                        .n1 = 16 - n * 2 - n_1x1, /// space -> 00
//                        .n2 = n - n_2x1, /// 1x2 -> 01
//                        .n3 = n_2x1, /// 2x1 -> 10
//                        .n4 = n_1x1, /// 1x1 -> 11
//                });

    return Group::block_num_t();
}

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

std::vector<RawCode> Group::group_cases(const RawCode &seed) {
    std::queue<uint64_t> cache;
    absl::flat_hash_map<uint64_t, uint64_t> cases; // <code, mask>
    cases.reserve(max_group_size(seed));
    cases.emplace(seed.unwrap(), 0); // without mask
    cache.emplace(seed.unwrap());

    auto core = Core(
        [&cache, &cases](auto &&code, auto &&mask) { // callback function
            auto current = cases.find(code);
            if (current != cases.end()) {
                current->second |= mask; // update mask
                return;
            }
            cases.emplace(code, mask);
            cache.emplace(code);
        }
    );
    while (!cache.empty()) { // until BFS without elements
        core.next_cases(cache.front(), cases.find(cache.front())->second);
        cache.pop(); // case dequeue
    }

    auto result = std::vector<RawCode>();
    result.reserve(cases.size());
    for (auto &&raw_code : cases) { // export group cases
        result.emplace_back(RawCode::unsafe_create(raw_code.first));
    }
    return result;
}




} // namespace klotski
