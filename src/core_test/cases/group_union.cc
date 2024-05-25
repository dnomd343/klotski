#include <algorithm>
#include <common_code.h>
#include <cstdint>
#include <iostream>

#include <format>
#include <utility.h>
#include <group/group.h>

#include <gtest/gtest.h>

struct block_num_t {
    int n_1x1;
    int n_1x2;
    int n_2x1;
};

template <>
struct std::hash<block_num_t> {
    size_t operator()(const block_num_t val) const noexcept {
        return std::hash<int>()(val.n_1x1) ^ std::hash<int>()(val.n_1x2) ^ std::hash<int>()(val.n_2x1);
    }
};

bool operator==(const block_num_t &lhs, const block_num_t &rhs) {
    return lhs.n_1x1 == rhs.n_1x1 && lhs.n_1x2 == rhs.n_1x2 && lhs.n_2x1 == rhs.n_2x1;
}

uint32_t cal_flag(const block_num_t &block_num) {

    return ((block_num.n_1x2 + block_num.n_2x1) << 7) | (block_num.n_2x1 << 3) | block_num.n_1x1;

}

std::vector<block_num_t> block_nums() {

    std::vector<std::pair<uint32_t, block_num_t>> tmp;

    for (int n = 0; n <= 7; ++n) {
        for (int n_1x2 = 0; n_1x2 <= n; ++n_1x2) {

            if (n == 7 && n_1x2 == 0) {
                continue;
            }

            for (int n_1x1 = 0; n_1x1 <= (20 - 4 - 2 - n * 2); ++n_1x1) {

                auto num = block_num_t {
                    .n_1x1 = n_1x1,
                    .n_1x2 = n_1x2,
                    .n_2x1 = n - n_1x2,
                };

                auto flag = cal_flag(num);

                tmp.emplace_back(flag, num);

                // std::cout << flag << std::endl;

            }
        }
    }

    std::sort(tmp.begin(), tmp.end(), [](const auto val1, const auto val2) {
        return val1.first < val2.first;
    });

    // for (auto [x, _] : tmp) {
    //     std::cout << x << std::endl;
    // }

    std::vector<block_num_t> results;
    std::ranges::transform(tmp, std::back_inserter(results), [](const auto val) {
        return val.second;
    });

    // for (auto x : results) {
    //     std::cout << std::format("n_1x1 = {} / n_1x2 = {} / n_2x1 = {}", x.n_1x1, x.n_1x2, x.n_2x1) << std::endl;
    // }

    return results;

}

block_num_t cal_block_num(const uint64_t common_code) noexcept {
    block_num_t result {};
    auto range = klotski::range_reverse(static_cast<uint32_t>(common_code));
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

uint32_t to_type_id(block_num_t block_num) {
    auto f = []() {
        auto ret = block_nums();

        std::unordered_map<block_num_t, uint32_t> kk;

        for (auto i = 0; i < ret.size(); ++i) {
            kk.emplace(ret[i], i);
        }

        return kk;

    };

    static auto data = f();

    return data[block_num];

}

TEST(GroupUnion, demo) {
    // kk();

    auto kk = cal_block_num(0x1A9BF0C00);
    std::cout << to_type_id(kk) << std::endl;

    // for (auto i = 0; i < block_nums().size(); ++i) {
    //     // std::cout << block_nums()[i].n_2x1 + block_nums()[i].n_1x2 << std::endl;
    //
    //     // std::cout << block_nums()[i].n_2x1 << std::endl;
    //
    //     std::cout << block_nums()[i].n_1x1 << std::endl;
    // }
    //
    // return;

    std::vector<std::vector<uint64_t>> pp;

    pp.resize(block_nums().size());

    // std::cout << pp.size() << std::endl;

    for (uint64_t head = 0; head < 16; ++head) {

        for (auto range : klotski::cases::AllCases::instance().fetch()[head]) {
            uint64_t common_code = head << 32 | range;

            auto type_id = to_type_id(cal_block_num(common_code));

            pp[type_id].emplace_back(common_code);

        }

    }

    // std::cout << pp[0].size() << std::endl;

    // std::cout << pp[169].size() << std::endl;

    for (uint32_t type_id = 0; type_id < klotski::cases::TYPE_ID_LIMIT; ++type_id) {

        // std::cout << "start cases cal" << std::endl;
        auto cases = klotski::cases::GroupUnion::unsafe_create(type_id).cases();
        // std::cout << "end cases cal" << std::endl;

        std::vector<uint64_t> extend {};
        for (uint64_t head = 0; head < 16; ++head) {
            for (auto range : cases[head]) {
                extend.emplace_back(head << 32 | range);
            }
        }

        std::cout << "type_id " << type_id << " -> " << extend.size() << std::endl;
        // std::cout << std::endl;

        EXPECT_EQ(extend, pp[type_id]);
    }


}
