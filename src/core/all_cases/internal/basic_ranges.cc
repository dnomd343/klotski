#include <list>
#include <algorithm>

#include "ranges/ranges.h"
#include "all_cases/all_cases.h"

using klotski::cases::Ranges;
using klotski::cases::BasicRanges;

typedef std::tuple<int, int, int> RangeType;
typedef std::array<RangeType, 204> RangeTypeUnion;

/// Generate all possible basic-ranges permutations.
consteval static RangeTypeUnion range_types() {
    RangeTypeUnion data;
    for (int i = 0, n = 0; n <= 7; ++n) { // 1x2 + 2x1 -> 0 ~ 7
        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) { // 2x1 -> 0 ~ n
            // TODO: skip n == 7 && n_2x1 == 7
            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) { // 1x1 -> 0 ~ (14 - 2n)
                data[i++] = {n, n_2x1, n_1x1};
            }
        }
    }
    return data;
}

/// Combine two consecutive sorted arrays into one sorted arrays.
static void inplace_merge(Ranges::iterator begin, Ranges::iterator mid, const Ranges::iterator end) {
    std::vector<uint32_t> tmp = {begin, mid}; // left array backup
    for (auto p = tmp.begin();;) {
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

void BasicRanges::build_ranges(Ranges &ranges) {
    ranges.clear();
    ranges.reserve(BASIC_RANGES_NUM);

    std::list flags { ranges.begin() };
    for (auto [n, n_2x1, n_1x1] : range_types()) {
        ranges.spawn_more(n, n_2x1, n_1x1);
        flags.emplace_back(ranges.end()); // mark ordered interval
    }

    do {
        decltype(flags.begin()) begin = flags.begin(), mid, end;
        while (++(mid = begin) != flags.end() && ++(end = mid) != flags.end()) {
            inplace_merge(*begin, *mid, *end); // merge two ordered interval
            flags.erase(mid);
            begin = end;
        }
    } while (flags.size() > 2); // merge until only one interval remains

    ranges.reverse(); // flip every 2-bit
}
