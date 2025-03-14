#include <ranges>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "ranges/ranges.h"
#include "helper/block_num.h"

using helper::block_num_t;
using klotski::range_reverse;

/// Build the sequence list of all block numbers.
static std::vector<block_num_t> build_block_nums() {
    std::vector<std::pair<uint32_t, block_num_t>> nums;
    for (int n = 0; n <= 7; ++n) {
        for (int n_1x2 = 0; n_1x2 <= n; ++n_1x2) {
            if (n == 7 && n_1x2 == 0) {
                continue;
            }
            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) {
                auto flag = (n << 7) | ((n - n_1x2) << 4) | n_1x1;
                nums.emplace_back(flag, block_num_t {
                    .n_1x1 = n_1x1,
                    .n_1x2 = n_1x2,
                    .n_2x1 = n - n_1x2,
                });
            }
        }
    }
    std::ranges::sort(nums.begin(), nums.end(), [](const auto lhs, const auto rhs) {
        return lhs.first < rhs.first;
    });
    return std::views::all(nums) | std::views::values | std::ranges::to<std::vector>();
}

/// Build the mapping table from block_num to type_id.
static std::unordered_map<block_num_t, uint32_t> build_type_id_map() {
    std::unordered_map<block_num_t, uint32_t> ids;
    for (uint64_t i = 0; i < helper::block_nums().size(); ++i) {
        ids.emplace(helper::block_nums()[i], i); // TODO: using `std::views::enumerate`
    }
    return ids;
}

const std::vector<block_num_t>& helper::block_nums() {
    static auto data = build_block_nums();
    return data;
}

block_num_t helper::cal_block_num(uint32_t range) {
    block_num_t result {};
    for (range = range_reverse(range); range; range >>= 2) {
        switch (range & 0b11) {
        case 0b01: // 1x2 block
            ++result.n_1x2;
            continue;
        case 0b10: // 2x1 block
            ++result.n_2x1;
            continue;
        case 0b11: // 1x1 block
            ++result.n_1x1;
            continue;
        default:; // space
        }
    }
    return result;
}

block_num_t helper::to_block_num(const uint32_t type_id) {
    if (type_id < helper::block_nums().size()) {
        return helper::block_nums()[type_id];
    }
    std::abort();
}

uint32_t helper::to_type_id(const block_num_t block_num) {
    static auto data = build_type_id_map();
    if (const auto match = data.find(block_num); match != data.end()) {
        return match->second;
    }
    std::abort();
}
