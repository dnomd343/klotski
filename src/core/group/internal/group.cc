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
static void demo(std::vector<uint32_t> &ranges, int n_10, int n_11) {

    constexpr auto num = 16 - N;
    constexpr auto offset = (16 - num) << 1; // offset of low bits

    int n_00 = 16 - N * 2 - n_11;
    int n_01 = N - n_10;

    std::array<int, num> series {};

    auto kk = std::fill_n(series.begin() + n_00, n_01, 0b01);
    auto pp = std::fill_n(kk, n_10, 0b10);
    std::fill_n(pp, n_11, 0b11);

    // std::vector<uint32_t> ranges;

    do {
        uint32_t range = 0;
        for (const auto x : series) // store every 2-bit
            (range <<= 2) |= x;
        ranges.emplace_back(range << offset);
    } while (std::ranges::next_permutation(series).found);

    // return ranges;
}

void klotski::cases::spawn_ranges(std::vector<uint32_t> &ranges, int n, int n_2x1, int n_1x1) {

    // auto n = n_01 + n_2x1;

    // std::vector<uint32_t> ranges;

    switch (n) {
        case 0: return demo<0>(ranges, n_2x1, n_1x1);
        case 1: return demo<1>(ranges, n_2x1, n_1x1);
        case 2: return demo<2>(ranges, n_2x1, n_1x1);
        case 3: return demo<3>(ranges, n_2x1, n_1x1);
        case 4: return demo<4>(ranges, n_2x1, n_1x1);
        case 5: return demo<5>(ranges, n_2x1, n_1x1);
        case 6: return demo<6>(ranges, n_2x1, n_1x1);
        case 7: return demo<7>(ranges, n_2x1, n_1x1);
        default: return;
    }

}

consteval std::array<std::tuple<int, int, int>, 204> target_nums() {
    std::array<std::tuple<int, int, int>, 204> results;
    for (int i = 0, n = 0; n <= 7; ++n) {
        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) {
            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) {
                results[i++] = {n, n_2x1, n_1x1};
            }
        }
    }
    return results;
}

using RangeIter = std::vector<uint32_t>::iterator;

static void combine_sort(RangeIter begin, RangeIter mid, RangeIter end) noexcept {

    // std::inplace_merge(begin, mid, end);
    // return;

    // std::vector<uint32_t> results;
    // results.resize(end - begin);
    // std::merge(begin, mid, mid, end, results.begin());
    // std::copy(results.begin(), results.end(), begin);
    // return;

    std::vector<uint32_t> tmp = {begin, mid}; // left array backup
    auto p = tmp.begin();
    for (;;) {
        if (*p <= *mid) {
            *(begin++) = *(p++); // stored in original span
            if (p == tmp.end()) // left array is consumed
                return;
            continue;
        }
        *(begin++) = *(mid++); // stored in original span
        if (mid == end) { // right array is consumed
            std::copy(p, tmp.end(), begin); // left array remaining
            return;
        }
    }
}

std::vector<uint32_t> klotski::cases::basic_ranges() {

    std::vector<uint32_t> results;
    results.reserve(7311921);

    std::list<std::vector<uint32_t>::iterator> flags {results.begin()}; // mark ordered interval

    for (auto [n, n_2x1, n_1x1] : target_nums()) {

        spawn_ranges(results, n, n_2x1, n_1x1);

        flags.emplace_back(results.end());

        // auto sub_ranges = spawn_ranges(results, n, n_2x1, n_1x1);

        // results.insert(results.end(), sub_ranges.begin(), sub_ranges.end());
    }

    // std::ranges::sort(results.begin(), results.end());
    // std::ranges::stable_sort(results.begin(), results.end());

    do {
        decltype(flags.begin()) begin = flags.begin(), mid, end;
        while (++(mid = begin) != flags.end() && ++(end = mid) != flags.end()) {
            combine_sort(*begin, *mid, *end); // merge two ordered interval
            flags.erase(mid);
            begin = end;
        }
    } while (flags.size() > 2); // merge until only one interval remains

    for (auto &x : results) {
        x = range_reverse(x);
    }

    return results;

    // std::vector<uint32_t> kk;
    // kk.reserve(7311921);
    // std::ranges::transform(results.begin(), results.end(), kk.begin(), range_reverse);
    // return kk;
}
