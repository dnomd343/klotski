#include <iostream>

#include "cases.h"

#include <ranges>

block_num_t cal_block_num(uint32_t range) {
    block_num_t result {};
    range = range_reverse(range);
    for (; range; range >>= 2) {
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
        }
    }
    return result;
}

static std::vector<block_num_t> build_block_nums() {
    auto cal_type_flag = [](const block_num_t &block_num) {
        const auto n_x2x = block_num.n_1x2 + block_num.n_2x1;
        return (n_x2x << 7) | (block_num.n_2x1 << 4) | block_num.n_1x1;
    };

    std::vector<std::pair<uint32_t, block_num_t>> nums;
    for (int n = 0; n <= 7; ++n) {
        for (int n_1x2 = 0; n_1x2 <= n; ++n_1x2) {
            if (n == 7 && n_1x2 == 0) {
                continue;
            }
            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) {
                auto num = block_num_t {
                    .n_1x1 = n_1x1,
                    .n_1x2 = n_1x2,
                    .n_2x1 = n - n_1x2,
                };
                nums.emplace_back(cal_type_flag(num), num);
            }
        }
    }

    std::ranges::sort(nums.begin(), nums.end(), [](const auto val1, const auto val2) {
        return val1.first < val2.first;
    });

    return std::views::all(nums) | std::views::transform([](const auto val) {
        return val.second;
    }) | std::ranges::to<std::vector>();
}

uint32_t to_type_id(block_num_t block_num) {
    auto f = []() {
        auto ret = build_block_nums();

        std::unordered_map<block_num_t, uint32_t> kk;

        for (auto i = 0; i < ret.size(); ++i) {
            kk.emplace(ret[i], i);
        }

        return kk;

    };

    static auto data = f();

    return data[block_num];

    // TODO: handle invalid params
}

block_num_t to_block_num(uint32_t type_id) {
    static auto data = build_block_nums();

    // TODO: handle invalid params

    return data[type_id];
}

std::vector<block_num_t> block_nums() {
    static auto data = build_block_nums();
    return data;
}
