#include "group/group.h"

#include <queue>

#include <absl/container/btree_map.h>
#include <absl/container/btree_set.h>
#include <absl/container/flat_hash_map.h>
#include <absl/container/node_hash_map.h>
#include <core/core.h>

static KLSK_INLINE uint32_t type_id(const int n, const int n_2x1, const int n_1x1) {
    constexpr int offset[8] = {0, 15, 41, 74, 110, 145, 175, 196};
    return offset[n] + (15 - n * 2) * n_2x1 + n_1x1;
}

uint32_t klotski::cases::common_code_to_type_id(const uint64_t common_code) {
    const auto range = static_cast<uint32_t>(common_code);
    const auto n_1x1 = std::popcount((range >> 1) & range & 0x55555555);
    const auto n_2x1 = std::popcount((range >> 1) & ~range & 0x55555555);
    return type_id(std::popcount(range) - n_1x1 * 2, n_2x1, n_1x1);
}

uint32_t klotski::cases::raw_code_to_type_id(const uint64_t raw_code) {
    const auto n = std::popcount(((raw_code >> 1) ^ raw_code) & 0x0249249249249249);
    const auto n_2x1 = std::popcount((raw_code >> 1) & ~raw_code & 0x0249249249249249);
    const auto n_1x1 = std::popcount((raw_code >> 1) & raw_code & 0x0249249249249249) - n - 3;
    return type_id(n, n_2x1, n_1x1);
}

std::vector<uint64_t> klotski::cases::group_extend_from_seed(uint64_t raw_code) {

    auto max_size = GroupUnion::create(raw_code_to_type_id(raw_code))->max_group_size();

    // std::queue<uint64_t> cache({raw_code});

    uint64_t offset = 0;
    std::vector<uint64_t> results;
    results.reserve(max_size);
    results.emplace_back(raw_code);

    // uint64_t offset = 0;
    // std::vector<std::pair<uint64_t, uint64_t>> results;
    // results.reserve(max_size);
    // results.emplace_back(raw_code, 0);

    absl::flat_hash_map<uint64_t, uint64_t> cases; // <code, mask>
    // absl::node_hash_map<uint64_t, uint64_t> cases; // <code, mask>
    // std::unordered_map<uint64_t, uint64_t> cases; // <code, mask>
    // std::map<uint64_t, uint64_t> cases; // <code, mask>
    // absl::btree_map<uint64_t, uint64_t> cases; // <code, mask>

    cases.reserve(max_size);
    cases.emplace(raw_code, 0); // without mask

    // std::cout << max_size << std::endl;

    auto core = klotski::core::Core(
        [&results, &cases](auto code, auto mask) { // callback function
            auto current = cases.find(code);
            if (current != cases.end()) {
                current->second |= mask; // update mask
                // results[current->second].second |= mask; // update mask
                return;
            }
            cases.emplace(code, mask);
            // cases.emplace(code, results.size());

            // cache.emplace(code);
            results.emplace_back(code);
            // cache.emplace(code, 0);
            // results.emplace_back(code, 0);
        }
    );

    while (offset != results.size()) {
        auto tmp = results[offset];
        core.next_cases(tmp, cases.find(tmp)->second);
        // core.next_cases(tmp, 0);

        // core.next_cases(tmp.first, tmp.second);

        ++offset;
    }

    return results;

    // return {};

    // while (!cache.empty()) { // until BFS without elements
    //     core.next_cases(cache.front(), cases.find(cache.front())->second);
    //     cache.pop(); // case dequeue
    // }

    // std::vector<uint64_t> result;
    // result.reserve(cases.size());
    // for (auto &&tmp : cases) { // export group cases
    //     result.emplace_back(tmp.first);
    // }
    // return result;

    // std::vector<uint64_t> result;
    // result.reserve(results.size());
    // for (auto [code, _] : results) {
    //     result.emplace_back(code);
    // }
    // return result;

}
