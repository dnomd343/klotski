#include <queue>
#include "core.h"
#include "group.h"
#include "common.h"
#include "type_id.h"
#include "common_code.h"
#include "absl/container/btree_set.h"
#include "absl/container/flat_hash_map.h"

namespace klotski {

using Common::check_range;
using Common::range_reverse;

/// --------------------------------------- Group Type ----------------------------------------

std::vector<CommonCode> GroupType::cases() const noexcept {
    std::vector<uint32_t> ranges; // basic ranges of type_id
    ranges.reserve(TYPE_ID_SIZE[type_id_]); // over-allocation

    auto tmp = block_num();
    BasicRanges::generate(ranges, { // generate target ranges
        .n1 = 16 - tmp.n_1x1 - (tmp.n_1x2 + tmp.n_2x1) * 2, /// space -> 00
        .n2 = tmp.n_1x2, /// 1x2 -> 01
        .n3 = tmp.n_2x1, /// 2x1 -> 10
        .n4 = tmp.n_1x1, /// 1x1 -> 11
    });
    for (auto &range : ranges) {
        range = range_reverse(range); // basic ranges reversed
    }

    std::vector<CommonCode> all_cases;
    all_cases.reserve(TYPE_ID_SIZE[type_id_]);
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

std::vector<std::vector<CommonCode>> GroupType::groups() const noexcept {
    auto all_cases = GroupType::cases();
    std::vector<std::vector<CommonCode>> groups;

    auto min = std::min_element(all_cases.begin(), all_cases.end()); // search min CommonCode
    auto first_group = Group::cases(min->to_raw_code()); // expand the first group
    groups.emplace_back(first_group.begin(), first_group.end());
    if (first_group.size() == all_cases.size()) {
        return groups; // only contains one group
    }

    absl::btree_set<CommonCode> cases(all_cases.begin(), all_cases.end());
    for (auto &&tmp : *groups.begin()) {
        cases.erase(tmp); // remove elements from first group
    }
    while (!cases.empty()) {
        auto group = Group::cases(cases.begin()->to_raw_code());
        groups.emplace_back(group.begin(), group.end()); // release new group
        for (auto &&tmp : *(groups.end() - 1)) {
            cases.erase(tmp); // remove selected cases
        }
    }

    auto cmp_func = [](const std::vector<CommonCode> &v1, const std::vector<CommonCode> &v2) {
        return v1.size() > v2.size(); // sort by vector size
    };
    std::stable_sort(groups.begin(), groups.end(), cmp_func); // using stable sort for ordered index
    return groups;
}

/// --------------------------------------- Group Size ----------------------------------------

uint32_t Group::size() const noexcept {
    return size(seed());
}

uint32_t Group::size(const CommonCode &common_code) noexcept {
    return size(common_code.to_raw_code());
}

uint32_t Group::size(const RawCode &raw_code) noexcept {
    std::queue<uint64_t> cache({raw_code.unwrap()});
    absl::flat_hash_map<uint64_t, uint64_t> cases; // <code, mask>
    cases.reserve(GroupType::max_size(raw_code));
    cases.emplace(raw_code.unwrap(), 0b0); // without mask

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
    return cases.size();
}

/// --------------------------------------- Group Cases ---------------------------------------

std::vector<RawCode> Group::cases() const noexcept {
    return cases(seed());
}

std::vector<RawCode> Group::cases(const CommonCode &common_code) noexcept {
    return cases(common_code.to_raw_code());
}

std::vector<RawCode> Group::cases(const RawCode &raw_code) noexcept {
    std::queue<uint64_t> cache({raw_code.unwrap()});
    absl::flat_hash_map<uint64_t, uint64_t> cases; // <code, mask>
    cases.reserve(GroupType::max_size(raw_code));
    cases.emplace(raw_code.unwrap(), 0b0); // without mask

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

    std::vector<RawCode> result;
    result.reserve(cases.size());
    for (auto &&tmp : cases) { // export group cases
        result.emplace_back(RawCode::unsafe_create(tmp.first));
    }
    return result;
}

} // namespace klotski
