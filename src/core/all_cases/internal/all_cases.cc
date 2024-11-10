#include "all_cases/all_cases.h"

using klotski::range_reverse;
using klotski::cases::Ranges;
using klotski::cases::AllCases;
using klotski::cases::BasicRanges;
using klotski::cases::ALL_CASES_NUM;

typedef std::array<int, 12> Heads;

/// Generate all possible klotski heads.
static consteval Heads get_heads() {
    Heads heads {};
    for (int i = 0, head = 0; head < 15; ++head) {
        if (head % 4 != 3) {
            heads[i++] = head;
        }
    }
    return heads;
}

/// Build all valid ranges of the specified head.
static void build_cases(const std::vector<uint32_t> &ranges,
                        const std::vector<uint32_t> &reversed, Ranges &release, const int head) {
    release.clear();
    release.reserve(ALL_CASES_NUM[head]);

    for (uint32_t index = 0; index < reversed.size(); ++index) {
        CHECK_NEXT:
        if (const auto offset = Ranges::check(head, reversed[index])) { // invalid case
            if (offset > 14) {
                ++index; // never overflow
                goto CHECK_NEXT;
            }
            //         !! <- broken
            // ( xx xx xx ) xx xx xx ... [range]
            //         +1   00 00 00 ... (delta)
            const int tmp = (16 - offset) * 2;
            const uint32_t min_next = ((ranges[index] >> tmp) + 1) << tmp; // next possible range

            // min_next always less than ranges.back()
            if (offset > 5) {
                while (ranges[++index] < min_next) {} // located next range
                goto CHECK_NEXT;
            }
            index = std::lower_bound(ranges.begin() + index, ranges.end(), min_next) - ranges.begin();
            goto CHECK_NEXT;
        }
        release.emplace_back(range_reverse(reversed[index])); // release valid case
    }
}

void AllCases::build() {
    if (available_) {
        return; // reduce consumption of mutex
    }
    const std::lock_guard guard {building_};
    if (available_) {
        return; // data is already available
    }

    const auto &ranges = BasicRanges::instance().fetch();
    std::vector reversed {ranges};
    for (auto &x : reversed) {
        x = range_reverse(x);
    }
    for (const auto head : get_heads()) {
        build_cases(ranges, reversed, get_cases().ranges(head), head);
    }
    available_ = true;
    KLSK_MEM_BARRIER;
}

void AllCases::build_async(Executor &&executor, Notifier &&callback) {
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

    auto &ranges = BasicRanges::instance().fetch();
    auto reversed = std::make_shared<std::vector<uint32_t>>(ranges);
    for (auto &x : *reversed) {
        x = range_reverse(x);
    }

    Worker worker {executor};
    for (const auto head : get_heads()) {
        worker.post([head, reversed] {
            build_cases(BasicRanges::instance().fetch(), *reversed, get_cases().ranges(head), head);
        });
    }

    worker.then([this, callback = std::move(callback)] {
        available_ = true;
        KLSK_MEM_BARRIER;
        building_.unlock();
        callback();
    });
}
