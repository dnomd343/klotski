#include <queue>
#include <vector>
#include "core.h"
#include "group.h"
#include "absl/container/flat_hash_map.h"
#include "common.h"

namespace klotski {

using klotski::AllCases;
using klotski::BasicRanges;

using klotski::Common::check_range;
using klotski::Common::range_reverse;

std::vector<CommonCode> Group::all_cases(uint32_t type_id) {
    auto tmp = block_num(type_id);
    std::vector<uint32_t> ranges; // basic ranges of type_id
    BasicRanges::generate(ranges, BasicRanges::generate_t { // generate target ranges
        .n1 = 16 - tmp.n_1x1 - (tmp.n_1x2 + tmp.n_2x1) * 2, /// space -> 00
        .n2 = tmp.n_1x2, /// 1x2 -> 01
        .n3 = tmp.n_2x1, /// 2x1 -> 10
        .n4 = tmp.n_1x1, /// 1x1 -> 11
    });
    for (auto &range : ranges) {
        range = range_reverse(range); // basic ranges reversed
    }

    std::vector<CommonCode> all_cases;
    // TODO: all_cases pre-allocated
    for (uint64_t head = 0; head < 15; ++head) { // address of 2x2 block
        /// head -> 0/1/2 / 4/5/6 / 8/9/10 / 12/13/14
        if ((head & 0b11) == 0b11) {
            ++head; // skip invalid address
        }
        /// head(4-bit) + basic-range(32-bit) --check--> valid cases
        for (auto &&range : ranges) {
            if (!check_range(head, range)) { // case valid
                all_cases.emplace_back(CommonCode::unsafe_create(
                    head << 32 | range_reverse(range) // release valid cases
                ));
            }
        }
    }
    return all_cases;
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
