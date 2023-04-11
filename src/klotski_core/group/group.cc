#include <queue>
#include <vector>
#include "core.h"
#include "group.h"
#include "absl/container/flat_hash_map.h"
#include "common.h"

#include "size.h"

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
    all_cases.reserve(GROUP_ALL_CASES_SIZE[type_id]);
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

std::vector<std::vector<CommonCode>> Group::build_groups(uint32_t type_id) {
    auto all_cases = Group::all_cases(type_id);
    if (all_cases.empty()) {
        return {}; // type 203 -> no element
    }

    std::vector<std::vector<CommonCode>> groups;
    auto min = std::min_element(all_cases.begin(), all_cases.end()); // search min CommonCode
    auto first_group = Group::group_cases(min->to_raw_code()); // expand the first group
    groups.emplace_back(first_group.begin(), first_group.end());
    if (first_group.size() == all_cases.size()) { // only contains one group
        return groups;
    }

    std::set<CommonCode> cases(all_cases.begin(), all_cases.end());
    for (auto &&tmp : groups[0]) {
        cases.erase(tmp); // remove elements in first group
    }
    while (!cases.empty()) {
        groups.emplace_back(); // create empty vector
        auto current_group = groups.end() - 1; // insert into latest
        for (auto &&tmp : Group::group_cases(cases.begin()->to_raw_code())) {
            auto common_code = tmp.to_common_code();
            current_group->emplace_back(common_code); // insert into current group
            cases.erase(common_code); // remove from global union
        }
    }

    auto compare_func = [](const std::vector<CommonCode> &v1, const std::vector<CommonCode> &v2) {
        return v1.size() < v2.size(); // sort by vector size
    };
    std::stable_sort(groups.begin(), groups.end(), compare_func); // using stable sort for ordered index
    return groups;
}

} // namespace klotski
