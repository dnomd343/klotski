#include <list>
#include <algorithm>

#include "ranges/ranges.h"
#include "utils/utility.h"

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

// void klotski::cases::spawn_ranges(std::vector<uint32_t> &ranges, int n, int n_2x1, int n_1x1) {
//
//
// }

void klotski::cases::Ranges::spawn_more(int n, int n_2x1, int n_1x1) {
    // spawn_ranges(ranges_, n, n_2x1, n_1x1);

    switch (n) {
    case 0: return demo<0>(ranges_, n_2x1, n_1x1);
    case 1: return demo<1>(ranges_, n_2x1, n_1x1);
    case 2: return demo<2>(ranges_, n_2x1, n_1x1);
    case 3: return demo<3>(ranges_, n_2x1, n_1x1);
    case 4: return demo<4>(ranges_, n_2x1, n_1x1);
    case 5: return demo<5>(ranges_, n_2x1, n_1x1);
    case 6: return demo<6>(ranges_, n_2x1, n_1x1);
    case 7: return demo<7>(ranges_, n_2x1, n_1x1);
    default: return;
    }

}
