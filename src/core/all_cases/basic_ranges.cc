#include <list>
#include <algorithm>

#include "utility.h"
#include "all_cases.h"

namespace klotski {
namespace cases {

typedef std::vector<Range>::iterator RangeIter;
typedef std::tuple<int, int, int, int> RangeType;
typedef std::array<RangeType, 204> RangeTypes;

/// Calculate all possible basic-ranges permutations.
consteval static RangeTypes range_types() {
    RangeTypes data;
    for (int i = 0, n = 0; n <= 7; ++n) // 1x2 and 2x1 -> 0 ~ 7
        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) // 2x1 -> 0 ~ n
            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) // 1x1 -> 0 ~ (14 - 2n)
                data[i++] = {16 - n * 2 - n_1x1, n - n_2x1, n_2x1, n_1x1};
    return data;
}

/// Combine two consecutive sorted arrays into one sorted arrays.
static void combine_sort(RangeIter begin, RangeIter mid, RangeIter end) noexcept {
    Ranges tmp = {begin, mid}; // left array backup
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

/// Spawn all ranges of specified conditions.
void BasicRanges::SpawnRanges(Ranges &ranges, int n1, int n2, int n3, int n4) noexcept {
    auto num = n1 + n2 + n3 + n4;
    auto offset = (16 - num) << 1; // offset of low bits

    std::vector<int> series;
    series.reserve(num);
    series.insert(series.end(), n1, 0b00);
    series.insert(series.end(), n2, 0b01);
    series.insert(series.end(), n3, 0b10);
    series.insert(series.end(), n4, 0b11);

    do { // full permutation traversal
        uint32_t range = 0;
        for (auto x : series) // store every 2-bit
            (range <<= 2) |= x;
        ranges.emplace_back(range << offset);
    } while (next_permutation(series.begin(), series.end()));
}

/// Search and sort all possible basic-ranges permutations.
void BasicRanges::BuildRanges(Ranges &ranges) noexcept {
    ranges.clear();
    ranges.reserve(BASIC_RANGES_NUM);
    std::list<RangeIter> flags {ranges.begin()}; // mark ordered interval

    for (auto &t : range_types()) {
        SpawnRanges(ranges, std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t));
        flags.emplace_back(ranges.end());
    }
    do {
        decltype(flags.begin()) begin = flags.begin(), mid, end;
        while (++(mid = begin) != flags.end() && ++(end = mid) != flags.end()) {
            combine_sort(*begin, *mid, *end); // merge two ordered interval
            flags.erase(mid);
            begin = end;
        }
    } while (flags.size() > 2); // merge until only one interval remains

    for (auto &x : ranges) {
        x = range_reverse(x); // flip every 2-bit
    }
}

/// Execute the build process and ensure thread safety.
void BasicRanges::Build() noexcept {
    if (available_) {
        return; // reduce consumption of mutex
    }
    if (building_.try_lock()) { // mutex lock success
        if (available_) {
            building_.unlock();
            return;
        }
        BuildRanges(GetRanges());
        available_ = true;
    } else {
        building_.lock(); // blocking waiting
    }
    building_.unlock(); // release mutex
}

Ranges& BasicRanges::GetRanges() noexcept {
    static Ranges ranges;
    return ranges;
}

BasicRanges& BasicRanges::Instance() noexcept {
    static BasicRanges instance;
    return instance;
}

const Ranges& BasicRanges::Fetch() noexcept {
    this->Build();
    return GetRanges();
}

bool BasicRanges::IsAvailable() const noexcept {
    return available_;
}

} // namespace cases
} // namespace klotski
