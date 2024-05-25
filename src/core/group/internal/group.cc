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

template<int N>
static std::vector<uint32_t> demo(int n_10, int n_11) {

    constexpr auto num = 16 - N;
    constexpr auto offset = (16 - num) << 1; // offset of low bits

    int n_00 = 16 - N * 2 - n_11;
    int n_01 = N - n_10;

    std::array<int, num> series {};

    // for (int k = 0; k < n_00; ++k) {
    //     series[k] = 0b00;
    // }
    // for (int k = n_00; k < n_00 + n_01; ++k) {
    //     series[k] = 0b01;
    // }

    auto kk = std::fill_n(series.begin() + n_00, n_01, 0b01);
    auto pp = std::fill_n(kk, n_10, 0b10);
    std::fill_n(pp, n_11, 0b11);

    // for (auto x : series) {
    //     std::cout << x << " ";
    // }
    // std::cout << std::endl;

    std::vector<uint32_t> ranges;

    do {
        uint32_t range = 0;
        for (const auto x : series) // store every 2-bit
            (range <<= 2) |= x;
        ranges.emplace_back(range << offset);
    } while (std::ranges::next_permutation(series).found);

    return ranges;
}

std::vector<uint32_t> klotski::cases::spawn_ranges(int n_00, int n_01, int n_10, int n_11) {

    auto n = n_01 + n_10;

    switch (n) {
        case 0: return demo<0>(n_10, n_11);
        case 1: return demo<1>(n_10, n_11);
        case 2: return demo<2>(n_10, n_11);
        case 3: return demo<3>(n_10, n_11);
        case 4: return demo<4>(n_10, n_11);
        case 5: return demo<5>(n_10, n_11);
        case 6: return demo<6>(n_10, n_11);
        case 7: return demo<7>(n_10, n_11);
        default: return {};
    }

    // return demo<5>(n_10, n_11);

    // auto num = n_00 + n_01 + n_10 + n_11;
    // auto offset = (16 - num) << 1; // offset of low bits

    // std::vector<int> series;
    // series.reserve(num);
    // series.insert(series.end(), n_00, 0b00);
    // series.insert(series.end(), n_01, 0b01);
    // series.insert(series.end(), n_10, 0b10);
    // series.insert(series.end(), n_11, 0b11);

    // std::array<int, 11> series {
    //     0b00, 0b00,
    //     0b01,
    //     0b10, 0b10, 0b10, 0b10,
    //     0b11, 0b11, 0b11, 0b11,
    // };

    // std::vector<uint32_t> ranges;

    // do { // full permutation traversal
    //     uint32_t range = 0;
    //     for (const auto x : series) // store every 2-bit
    //         (range <<= 2) |= x;
    //     ranges.emplace_back(range << offset);
    // } while (std::next_permutation(series.begin(), series.end()));

    // do {
    //     uint32_t range = 0;
    //     for (const auto x : series) // store every 2-bit
    //         (range <<= 2) |= x;
    //     ranges.emplace_back(range << offset);
    // } while (std::ranges::next_permutation(series).found);

    // return ranges;
}
