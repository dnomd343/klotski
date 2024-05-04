#include <future>

#include "all_cases/all_cases.h"

namespace klotski::cases {

/// Calculate all possible klotski heads.
consteval static std::array<int, 12> case_heads() {
    std::array<int, 12> heads = {};
    for (int i = 0, head = 0; head < 15; ++head) {
        if (head % 4 != 3) {
            heads[i++] = head;
        }
    }
    return heads;
}

/// Check whether the combination of head and range is valid.
static int check_range(const int head, uint32_t range) noexcept {
    constexpr uint32_t M_1x1 = 0b00000001;
    constexpr uint32_t M_1x2 = 0b00000011;
    constexpr uint32_t M_2x1 = 0b00010001;
    constexpr uint32_t M_2x2 = 0b00110011;

    uint32_t flags = M_2x2 << head; // fill 2x2 block
    for (int addr = 0, offset = 1; range; range >>= 2, ++offset) { // traverse every 2-bit
        const auto num = low_zero_num(~flags);
        addr += num; // next unfilled block
        flags >>= num;
        switch (range & 0b11) {
            case 0b00: // space
            case 0b11: // 1x1 block
                flags |= M_1x1;
                continue;
            case 0b10: // 2x1 block
                if ((flags >> 4) & 0b1 || addr > 15) { // invalid case
                    return offset; // broken offset
                }
                flags |= M_2x1;
                continue;
            case 0b01: // 1x2 block
                if ((flags >> 1) & 0b1 || (addr & 0b11) == 0b11) { // invalid case
                    return offset; // broken offset
                }
                flags |= M_1x2;
                continue;
        }
    }
    return 0; // pass check
}

void AllCases::build_cases(const int head, Ranges &release) {
    release.clear();
    release.reserve(ALL_CASES_NUM[head]);
    auto &basic_ranges = BasicRanges::instance().fetch();
    for (uint32_t index = 0; index < basic_ranges.size(); ++index) {
        auto offset = check_range(head, basic_ranges[index]);
        if (offset) { // invalid case
            auto tmp = (uint32_t)0b1 << (32 - offset * 2); // distance to next possible range
            ///         !! <- broken
            /// ( xx xx xx ) xx xx xx ... [reversed range]
            ///         +1   00 00 00 ...     (delta)
            tmp += range_reverse(basic_ranges[index]) & ~(tmp - 1);
            while (range_reverse(basic_ranges[++index]) < tmp); // located next range
            --index;
            continue;
        }
        release.emplace_back(range_reverse(basic_ranges[index])); // release valid case
    }
}

void AllCases::build() {
    build_parallel([](auto &&func) {
        func();
    });
}

void AllCases::build_parallel(Executor &&executor) {
    if (available_) {
        return; // reduce consumption of mutex
    }
    std::lock_guard<std::mutex> guard(building_);
    if (available_) {
        return; // data is already available
    }
    std::vector<std::future<void>> futures;
    for (auto head : case_heads()) {
        auto promise = std::make_shared<std::promise<void>>();
        futures.emplace_back(promise->get_future());
        executor([head, promise = std::move(promise)]() {
            build_cases(head, get_cases()[head]);
            promise->set_value(); // subtask completed notification
        });
    }
    for (auto &x : futures) {
        x.get(); // wait until all subtasks completed
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
    auto counter = std::make_shared<std::atomic<int>>(0);
    auto all_done = std::make_shared<Notifier>(std::move(callback));
    for (auto head : case_heads()) {
        executor([this, head, counter, all_done]() {
            build_cases(head, get_cases()[head]);
            if (counter->fetch_add(1) == case_heads().size() - 1) { // all tasks done
                available_ = true;
                building_.unlock(); // release building mutex
                all_done->operator()(); // trigger callback
            }
        });
    }
}

} // namespace klotski::cases
