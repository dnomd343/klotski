#include <list>
#include <algorithm>

#include "group/group.h"
#include "all_cases/all_cases.h"

using klotski::cases::Ranges;
using klotski::cases::BasicRanges;

using klotski::group::BLOCK_NUM;
using klotski::group::TYPE_ID_LIMIT;

using RangesIter = Ranges::iterator;

/// Combine two consecutive sorted arrays into one sorted arrays.
static void inplace_merge(RangesIter begin, RangesIter mid, const RangesIter end) {
    std::vector<Ranges::value_type> tmp {begin, mid}; // left array backup
    for (auto p = tmp.begin();;) {
        if (*p <= *mid) {
            *(begin++) = *(p++); // stored in original span
            if (p == tmp.end()) { // left array is consumed
                return;
            }
            continue;
        }
        *(begin++) = *(mid++); // stored in original span
        if (mid == end) { // right array is consumed
            std::copy(p, tmp.end(), begin); // left array remaining
            return;
        }
    }
}

void BasicRanges::build() {
    if (available_) {
        return; // reduce consumption of mutex
    }
    const std::lock_guard guard {building_};
    if (available_) {
        return; // data is already available
    }

    auto &ranges = get_ranges();
    ranges.clear();
    ranges.reserve(BASIC_RANGES_NUM_);
    for (auto [n, n_2x1, n_1x1] : BLOCK_NUM) {
        ranges.spawn(n, n_2x1, n_1x1);
    }

    std::list<RangesIter> points; // mark ordered interval
    for (const auto offset : BASIC_RANGES_OFFSET) {
        points.emplace_back(ranges.begin() + offset);
    }
    points.emplace_back(ranges.end());

    do {
        decltype(points)::iterator begin = points.begin(), mid, end;
        while (++(mid = begin) != points.end() && ++(end = mid) != points.end()) {
            inplace_merge(*begin, *mid, *end); // merge two ordered interval
            points.erase(mid);
            begin = end;
        }
    } while (points.size() > 2); // merge until only one interval remains

    available_ = true;
    KLSK_MEM_BARRIER;
}

void BasicRanges::build_async(Executor &&executor, Notifier &&callback) {
    if (available_) {
        callback();
        return; // reduce consumption of mutex
    }
    building_.lock();
    if (available_) {
        building_.unlock();
        callback();
        return; // data is already available
    }

    auto all_done = [this, callback = std::move(callback)] {
        available_ = true;
        KLSK_MEM_BARRIER;
        building_.unlock();
        callback();
    };

    Worker worker {executor};
    auto cache = std::make_shared<std::array<Ranges, TYPE_ID_LIMIT>>();
    for (uint32_t i = 0; i < TYPE_ID_LIMIT; ++i) {
        (*cache)[i].reserve(BASIC_RANGES_NUM[i]);
        worker.post([cache, i] {
            auto [n, n_2x1, n_1x1] = BLOCK_NUM[i];
            (*cache)[i].spawn(n, n_2x1, n_1x1);
        });
    }

    worker.then([cache, all_done = std::move(all_done), executor = std::move(executor)] mutable {
        auto &ranges = get_ranges();
        ranges.clear();
        ranges.reserve(BASIC_RANGES_NUM_);
        for (auto &&tmp : *cache) {
            ranges.insert(ranges.end(), tmp.begin(), tmp.end());
        }

        auto points = std::make_shared<std::list<RangesIter>>(); // mark ordered interval
        for (const auto offset : BASIC_RANGES_OFFSET) {
            points->emplace_back(ranges.begin() + offset);
        }
        points->emplace_back(ranges.end());

        auto inner_sort = [points, all_done, executor = std::move(executor)](auto &&self) -> void {
            Worker sorter {executor};

            auto begin = points->begin();
            decltype(begin) mid, end;
            while (++(mid = begin) != points->end() && ++(end = mid) != points->end()) {
                sorter.post([begin = *begin, mid = *mid, end = *end] {
                    inplace_merge(begin, mid, end); // merge two ordered interval
                });
                points->erase(mid);
                begin = end;
            }

            sorter.then([self, points, all_done] {
                if (points->size() == 2) {
                    all_done();
                    return;
                }
                self(self); // next sort round
            });
        };
        inner_sort(inner_sort); // TODO: using `this auto &&self` in new compiler
    });
}
