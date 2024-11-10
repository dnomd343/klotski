#include <algorithm>

#include "utils/utility.h"
#include "ranges/ranges.h"

using klotski::cases::Ranges;

template<int N>
static KLSK_INLINE void spawn_ranges(std::vector<uint32_t> &ranges, int n_10, int n_11) {
    const int n_01 = N - n_10;
    const int n_00 = 16 - N * 2 - n_11;

    std::array<int, 16 - N> series {};
    std::fill_n(series.begin() + n_00, n_01, 0b01);
    std::fill_n(series.begin() + n_00 + n_01, n_10, 0b10);
    std::fill_n(series.begin() + n_00 + n_01 + n_10, n_11, 0b11);

    do {
        uint32_t range = 0;
        for (const auto x : series) { // store every 2-bit
            (range <<= 2) |= x;
        }
        ranges.emplace_back(range << (N * 2));
    } while (std::ranges::next_permutation(series).found);
}

void Ranges::spawn(const int n, const int n_2x1, const int n_1x1) {
    KLSK_ASSUME(n >= 0 && n_2x1 >= 0 && n_1x1 >= 0);
    KLSK_ASSUME(n <= 7 && n_2x1 <= n && n_1x1 + n * 2 <= 14);
    switch (n) {
        case 0: return spawn_ranges<0>(*this, n_2x1, n_1x1);
        case 1: return spawn_ranges<1>(*this, n_2x1, n_1x1);
        case 2: return spawn_ranges<2>(*this, n_2x1, n_1x1);
        case 3: return spawn_ranges<3>(*this, n_2x1, n_1x1);
        case 4: return spawn_ranges<4>(*this, n_2x1, n_1x1);
        case 5: return spawn_ranges<5>(*this, n_2x1, n_1x1);
        case 6: return spawn_ranges<6>(*this, n_2x1, n_1x1);
        case 7: return spawn_ranges<7>(*this, n_2x1, n_1x1);
        default: std::unreachable();
    }
}
