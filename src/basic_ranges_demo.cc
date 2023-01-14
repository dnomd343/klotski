#include "basic_ranges_demo.h"

#include <algorithm>

#include <iostream>
#include <vector>
#include <queue>

#include "common.h"

std::vector<uint32_t> unsort_data;
std::vector<uint32_t> release_data;

void func_test(int n1, int n2, int n3, int n4) {

    struct inner_t {
        /// n4       n3       n2       n1
        /// 00000000 00000000 00000000 00000000
        uint32_t n;
        uint32_t prefix;
        int offset;
    };

    constexpr uint32_t MASK_01 = (uint32_t)0b01 << 30;
    constexpr uint32_t MASK_10 = (uint32_t)0b10 << 30;
    constexpr uint32_t MASK_11 = (uint32_t)0b11 << 30;

    std::queue<inner_t> data;

    data.emplace(inner_t {
        .n = static_cast<uint32_t>(n1 | n2 << 8 | n3 << 16 | n4 << 24),
        .prefix = 0x00000000,
        .offset = 0,
    });

    while (!data.empty()) {
        if (!data.front().n) {

            /// range release
//            release_data.emplace_back(data.front().prefix);
            unsort_data.emplace_back(data.front().prefix);

            data.pop();
            continue;
        }

        if (data.front().n & 0xFF) {
            auto next = inner_t {
                .n = data.front().n - 0x00000001,
                .prefix = data.front().prefix,
                .offset = data.front().offset + 2,
            };
            data.emplace(next);
        }
        if (data.front().n & 0xFF00) {
            auto next = inner_t {
                .n = data.front().n - 0x00000100,
                .prefix = data.front().prefix | (MASK_01 >> data.front().offset),
                .offset = data.front().offset + 2,
            };
            data.emplace(next);
        }
        if (data.front().n & 0xFF0000) {
            auto next = inner_t {
                .n = data.front().n - 0x00010000,
                .prefix = data.front().prefix | (MASK_10 >> data.front().offset),
                .offset = data.front().offset + 2,
            };
            data.emplace(next);
        }
        if (data.front().n & 0xFF000000) {
            auto next = inner_t {
                .n = data.front().n - 0x01000000,
                .prefix = data.front().prefix | (MASK_11 >> data.front().offset),
                .offset = data.front().offset + 2,
            };
            data.emplace(next);
        }
        data.pop();
    }
}

void load_ranges() {
//    std::cout << "ok" << std::endl;

    unsort_data.reserve(7311921);
    release_data.reserve(7311921);

    std::vector<int> start_point;

    for (int n = 0; n <= 7; ++n) { // number of 1x2 and 2x1 block -> 0 ~ 7
        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) { // number of 2x1 block -> 0 ~ n
            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) { // number of 1x1 block -> 0 ~ (14 - 2n)
                int n_1x2 = n - n_2x1;
                int n_space = 16 - n * 2 - n_1x1;

                start_point.emplace_back(unsort_data.size());

                func_test(n_space, n_1x2, n_2x1, n_1x1);

                /// 0x0 -> 00 | 1x2 -> 01 | 2x1 -> 10 | 1x1 -> 11
            }
        }
    }
    start_point.emplace_back(unsort_data.size());

    /// self-build sort

    struct node {
        uint32_t value;
        int index;
        int limit;
    };

    struct compare {
        bool operator() (node node1, node node2) {
            return node1.value > node2.value;
        }
    };

    std::priority_queue<node, std::vector<node>, compare> min_heap;

    for (int i = 0; i < start_point.size() - 1; ++i) {
        min_heap.push({
            .value = unsort_data[start_point[i]],
            .index = start_point[i],
            .limit = start_point[i + 1] - 1,
        });
    }

    while (!min_heap.empty()) {
        auto current = min_heap.top();
        min_heap.pop();

        release_data.emplace_back(current.value);

        if (current.index != current.limit) {
            min_heap.push({
                .value = unsort_data[current.index + 1],
                .index = current.index + 1,
                .limit = current.limit,
            });
        }
    }


    /// stl quick sort
//    sort(unsort_data.begin(), unsort_data.end());
//    release_data.insert(release_data.begin(), unsort_data.begin(), unsort_data.end());

    std::cout << "size: " << release_data.size() << std::endl;

//    for (const auto &range: release_data) {
//        printf("%08X\n", Common::range_reverse(range));
//    }

}
