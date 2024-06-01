#include "group/group.h"

// #include <queue>

// #include <absl/container/btree_set.h>
// #include <absl/container/flat_hash_map.h>
// #include <absl/container/node_hash_map.h>

#include <iostream>
#include <core/core.h>

static KLSK_INLINE uint32_t type_id(const int n, const int n_2x1, const int n_1x1) {
    constexpr int offset[8] = {0, 15, 41, 74, 110, 145, 175, 196};
    return offset[n] + (15 - n * 2) * n_2x1 + n_1x1;
}

static uint32_t common_code_to_type_id(const uint64_t common_code) {
    const auto range = static_cast<uint32_t>(common_code);
    const auto n_1x1 = std::popcount((range >> 1) & range & 0x55555555);
    const auto n_2x1 = std::popcount((range >> 1) & ~range & 0x55555555);
    return type_id(std::popcount(range) - n_1x1 * 2, n_2x1, n_1x1);
}

static uint32_t raw_code_to_type_id(const uint64_t raw_code) {
    const auto n = std::popcount(((raw_code >> 1) ^ raw_code) & 0x0249249249249249);
    const auto n_2x1 = std::popcount((raw_code >> 1) & ~raw_code & 0x0249249249249249);
    const auto n_1x1 = std::popcount((raw_code >> 1) & raw_code & 0x0249249249249249) - n - 3;
    return type_id(n, n_2x1, n_1x1);
}

uint32_t klotski::cases::GroupUnion::type_id(codec::CommonCode common_code) {
    return common_code_to_type_id(common_code.unwrap());
}

uint32_t klotski::cases::GroupUnion::type_id(codec::RawCode raw_code) {
    return raw_code_to_type_id(raw_code.unwrap());
}

// TODO: using {n, n_2x1, n_1x1} for CPU cache-line perf.

constexpr int TYPE_ID_N_NUM[203] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7,
};

constexpr int TYPE_ID_N_2x1_NUM[203] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1,
    1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
    2, 2, 2, 2, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
    2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4,
    4, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 0,
    0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5,
    5, 6, 6, 6, 0, 1, 2, 3, 4, 5, 6,
};

constexpr int TYPE_ID_N_1x1_NUM[203] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0,
    1, 2, 3, 4, 5, 6, 7, 0, 8, 8, 9, 1, 9, 10, 10, 2,
    11, 3, 11, 12, 12, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6,
    7, 8, 8, 0, 9, 1, 9, 2, 10, 10, 3, 4, 5, 6, 7, 0,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 1, 2, 3, 4, 5,
    6, 7, 8, 8, 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3,
    4, 5, 6, 7, 0, 8, 8, 1, 2, 3, 4, 5, 6, 7, 0, 1,
    2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3,
    4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5,
    6, 0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 0, 1, 2, 3, 4,
    0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 0,
    1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1,
    2, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0,
};

klotski::cases::RangesUnion klotski::cases::GroupUnion::cases() const {
    Ranges ranges {};
    int n = TYPE_ID_N_NUM[type_id_];
    int n_2x1 = TYPE_ID_N_2x1_NUM[type_id_];
    int n_1x1 = TYPE_ID_N_1x1_NUM[type_id_]; // TODO: cal from type_id
    ranges.spawn(n, n_2x1, n_1x1);

    // for (int i = 0; i < TYPE_ID_LIMIT; ++i) {
    //     ranges.spawn(TYPE_ID_N_NUM[i], TYPE_ID_N_2x1_NUM[i], TYPE_ID_N_1x1_NUM[i]);
    // }
    // std::stable_sort(ranges.begin(), ranges.end());

    for (auto &x : ranges) {
        x = klotski::range_reverse(x);
    }

    RangesUnion cases;
    ranges.derive(0x0, cases[0x0]);
    ranges.derive(0x1, cases[0x1]);
    ranges.derive(0x2, cases[0x2]);

    ranges.derive(0x4, cases[0x4]);
    ranges.derive(0x5, cases[0x5]);
    ranges.derive(0x6, cases[0x6]);

    ranges.derive(0x8, cases[0x8]);
    ranges.derive(0x9, cases[0x9]);
    ranges.derive(0xA, cases[0xA]);

    ranges.derive(0xC, cases[0xC]);
    ranges.derive(0xD, cases[0xD]);
    ranges.derive(0xE, cases[0xE]);

    return cases;
}
