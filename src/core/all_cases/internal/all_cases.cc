#include <future>

#include "utils/utility.h"
#include "ranges/ranges.h"
#include "all_cases/all_cases.h"

using klotski::range_reverse;
using klotski::cases::Ranges;
using klotski::cases::AllCases;
using klotski::cases::BasicRanges;
using klotski::cases::ALL_CASES_NUM;

/// Generate all possible klotski heads.
static consteval std::array<int, 12> heads() {
    std::array<int, 12> heads {};
    for (int i = 0, head = 0; head < 15; ++head) {
        if (head % 4 != 3) {
            heads[i++] = head;
        }
    }
    return heads;
}

/// Check whether the combination of head and range is valid.
static int check_range(const int head, uint32_t range) {
    uint32_t flags = 0b110011 << head; // fill 2x2 block
    for (int addr = 0, offset = 1; range; range >>= 2, ++offset) { // traverse every 2-bit
        const auto num = std::countr_one(flags);
        addr += num; // next unfilled block
        flags >>= num;
        switch (range & 0b11) {
            case 0b00: // space
            case 0b11: // 1x1 block
                flags |= 0b1;
                continue;
            case 0b01: // 1x2 block
                if (flags & 0b10 || addr % 4 == 3) { // invalid case
                    return offset; // broken offset
                }
                flags |= 0b11;
                continue;
            case 0b10: // 2x1 block
                if (flags & 0b10000 || addr > 15) { // invalid case
                    return offset; // broken offset
                }
                flags |= 0b10001;
        }
    }
    return 0; // pass check
}

/// Build all valid ranges of the specified head.
static void build_cases(const std::vector<uint32_t> &ranges,
                        const std::vector<uint32_t> &reversed, Ranges &release, const int head) {
    release.clear();
    release.reserve(ALL_CASES_NUM[head]);

    for (uint32_t index = 0; index < reversed.size(); ++index) {
        if (const auto offset = check_range(head, reversed[index])) { // invalid case
            if (offset > 14) {
                continue;
            }

            //         !! <- broken
            // ( xx xx xx ) xx xx xx ... [range]
            //         +1   00 00 00 ... (delta)
            const int tmp = (16 - offset) * 2;
            uint32_t min_next = ((ranges[index] >> tmp) + 1) << tmp; // next possible range

            if (offset > 5) { // located next range by min_next
                while (ranges[++index] < min_next) {}
            } else {
                index = std::lower_bound(ranges.begin() + index, ranges.end(), min_next) - ranges.begin();
            }
            --index;
            continue;
        }
        release.emplace_back(range_reverse(reversed[index])); // release valid case
    }
}

void AllCases::build() {
    if (available_) {
        return; // reduce consumption of mutex
    }

    std::lock_guard guard {building_};
    if (available_) {
        return; // data is already available
    }

    const auto &ranges = BasicRanges::instance().fetch();
    std::vector reversed {ranges};
    for (auto &x : reversed) {
        x = range_reverse(x);
    }
    for (const auto head : heads()) {
        build_cases(ranges, reversed, get_cases()[head], head);
    }
    available_ = true;
}

void AllCases::build_parallel_async(Executor &&executor, Notifier &&callback) {
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

    const auto counter = std::make_shared<std::atomic<int>>(0);
    const auto all_done = std::make_shared<Notifier>(std::move(callback));
    const auto reversed = std::make_shared<std::vector<uint32_t>>(BasicRanges::instance().fetch());
    for (auto &x : *reversed) {
        x = range_reverse(x);
    }

    for (const auto head : heads()) {
        executor([=, this] {
            build_cases(BasicRanges::instance().fetch(), *reversed, get_cases()[head], head);
            if (counter->fetch_add(1) == heads().size() - 1) { // all tasks done
                available_ = true;
                building_.unlock(); // release building mutex
                all_done->operator()(); // trigger callback
            }
        });
    }
}
