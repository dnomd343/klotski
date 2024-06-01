#include <list>
#include <algorithm>

#include "ranges/ranges.h"
#include "all_cases/all_cases.h"

using klotski::cases::Ranges;
using klotski::cases::BasicRanges;

typedef std::tuple<int, int, int> RangeType;
typedef std::array<RangeType, 203> RangeTypeUnion;

/// Generate all possible basic-ranges permutations.
consteval static RangeTypeUnion range_types() {
    RangeTypeUnion data;
    for (int i = 0, n = 0; n <= 7; ++n) { // 1x2 + 2x1 -> 0 ~ 7
        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) { // 2x1 -> 0 ~ n
            if (n == 7 && n_2x1 == 7) {
                break;
            }
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

    std::list flags {ranges.begin()}; // TODO: flags can be constexpr
    for (auto [n, n_2x1, n_1x1] : range_types()) {
        ranges.spawn(n, n_2x1, n_1x1);
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
}

void do_sort(klotski::Executor &&executor, klotski::Notifier notifier, std::shared_ptr<std::list<Ranges::iterator>> flags) {

    klotski::Worker worker {std::move(executor)};

    decltype(flags->begin()) begin = flags->begin(), mid, end;
    while (++(mid = begin) != flags->end() && ++(end = mid) != flags->end()) {

        worker.post([begin = *begin, mid = *mid, end = *end]() {
            inplace_merge(begin, mid, end); // merge two ordered interval
        });

        flags->erase(mid);
        begin = end;
    }

    worker.then([flags, notifier](klotski::Executor &&executor) {

        if (flags->size() == 2) {
            notifier();
            return;
        }

        do_sort(std::move(executor), notifier, flags);

    });

}

void BasicRanges::build_async(Executor &&executor, Notifier &&callback) {

    // TODO: add mutex protect here

    Worker worker {std::move(executor)};
    auto cache = std::make_shared<std::array<Ranges, 203>>();

    for (uint32_t i = 0; i < 203; ++i) {
        worker.post([cache, i] {
            auto [n, n_2x1, n_1x1] = range_types()[i];
            cache->operator[](i).spawn(n, n_2x1, n_1x1);
        });
    }

    // auto all_done = std::make_shared<Notifier>(std::move(callback));

    worker.then([cache, this, callback](Executor &&executor) {

        auto &ranges = get_ranges();

        ranges.clear();
        ranges.reserve(BASIC_RANGES_NUM);

        const auto flags = std::make_shared<std::list<Ranges::iterator>>();
        flags->emplace_back(ranges.end());

        for (auto &tmp : *cache) {
            ranges.insert(ranges.end(), tmp.begin(), tmp.end());
            flags->emplace_back(ranges.end()); // mark ordered interval
        }

        do_sort(std::move(executor), callback, flags);

        available_ = true;

    });
}
