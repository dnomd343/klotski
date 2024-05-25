#include "group/group.h"

#include <queue>

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

    uint64_t offset = 0;
    std::vector<uint64_t> results;
    results.reserve(max_size);
    results.emplace_back(raw_code);

    absl::flat_hash_map<uint64_t, uint64_t> cases; // <code, mask>
    cases.reserve(max_size);
    cases.emplace(raw_code, 0); // without mask

    auto core = klotski::core::Core(
        [&results, &cases](auto code, auto mask) { // callback function
            auto current = cases.find(code);
            if (current != cases.end()) {
                current->second |= mask; // update mask
                return;
            }
            cases.emplace(code, mask);
            results.emplace_back(code);
        }
    );

    while (offset != results.size()) {
        auto tmp = results[offset];
        core.next_cases(tmp, cases.find(tmp)->second);
        ++offset;
    }

    return results;

}
