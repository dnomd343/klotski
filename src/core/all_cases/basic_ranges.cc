#include <list>
#include <vector>
#include <algorithm>

#include "basic_ranges.h"

#include <iostream>

typedef uint32_t Range;
typedef std::vector<Range> Ranges;
typedef std::vector<Range>::iterator RangeIter;
typedef std::tuple<int, int, int, int> RangeType;

static const auto RangeTypeNum = 204;

static const uint32_t BASIC_RANGES_NUM = 7311921;

class BasicRanges {
public:
    static void build_ranges();

    static const Ranges& fetch();

private:
    static Ranges data_;

    static void spawn_ranges(int, int, int, int);
};

Ranges BasicRanges::data_;

static Range range_reverse(Range bin) noexcept {
#if defined(__GNUC__) || defined(__clang__)
    bin = __builtin_bswap32(bin);
    // TODO: using `std::byteswap` (c++23)
#else
    // FIXME: `_byteswap_ulong` under MSVC
    bin = ((bin << 16) & 0xFFFF0000) | ((bin >> 16) & 0x0000FFFF);
    bin = ((bin << 8) & 0xFF00FF00) | ((bin >> 8) & 0x00FF00FF);
#endif
    bin = ((bin << 4) & 0xF0F0F0F0) | ((bin >> 4) & 0x0F0F0F0F);
    return ((bin << 2) & 0xCCCCCCCC) | ((bin >> 2) & 0x33333333);
}

consteval static std::array<RangeType, RangeTypeNum> basic_types() {
    std::array<RangeType, RangeTypeNum> data;
    for (int i = 0, n = 0; n <= 7; ++n) // number of 1x2 and 2x1 block -> 0 ~ 7
        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) // number of 2x1 block -> 0 ~ n
            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) // number of 1x1 block -> 0 ~ (14 - 2n)
                data[i++] = {16 - n * 2 - n_1x1, n - n_2x1, n_2x1, n_1x1};
    return data;
}

static void combine_sort(RangeIter begin, RangeIter mid, RangeIter end) {
    Ranges tmp = {begin, mid};
    auto p = tmp.begin();
    for (;;) {
        if (*p < *mid) {
            *(begin++) = *(p++);
            if (p == tmp.end())
                return;
        } else {
            *(begin++) = *(mid++);
            if (mid == end) {
                std::copy(p, tmp.end(), begin);
                return;
            }
        }
    }
}

void BasicRanges::spawn_ranges(int n1, int n2, int n3, int n4) {
    std::vector<int> series;
    auto n = n1 + n2 + n3 + n4;
    auto offset = (16 - n) << 1;
    series.insert(series.end(), n1, 0b00);
    series.insert(series.end(), n2, 0b01);
    series.insert(series.end(), n3, 0b10);
    series.insert(series.end(), n4, 0b11);

    do {
        uint32_t range = 0;
        for (auto x : series)
            (range <<= 2) |= x;
        data_.emplace_back(range << offset);
    } while (next_permutation(series.begin(), series.end()));
}

void BasicRanges::build_ranges() {
    data_.reserve(BASIC_RANGES_NUM);
    std::list<RangeIter> flags {data_.begin()};

    for (auto &t : basic_types()) {
        spawn_ranges(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t));
        flags.emplace_back(data_.end());
    }
    do {
        std::list<RangeIter>::iterator begin = flags.begin(), mid, end;
        while (++(mid = begin) != flags.end() && ++(end = mid) != flags.end()) {
            combine_sort(*begin, *mid, *end);
            flags.erase(mid);
            begin = end;
        }
    } while (flags.size() > 2);

    for (auto &x : data_) {
        x = range_reverse(x);
    }
}

const Ranges& BasicRanges::fetch() {
    return data_;
}

void demo() {
    BasicRanges::build_ranges();

    for (auto x : BasicRanges::fetch()) {
        printf("%08X\n", x);
    }
}
