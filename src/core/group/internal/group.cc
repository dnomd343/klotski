#include "group/group.h"

#include <queue>

#include <absl/container/btree_set.h>
#include <absl/container/flat_hash_map.h>
#include <absl/container/node_hash_map.h>

#include <core/core.h>

std::vector<uint64_t> klotski::cases::Group::extend(codec::RawCode raw_code) {

    auto max_size = GroupUnion::from_raw_code(raw_code).max_group_size();
    // auto max_size = GroupUnion::create(raw_code_to_type_id(raw_code))->max_group_size();

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
