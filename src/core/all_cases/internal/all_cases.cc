#include <future>

#include "all_cases/all_cases.h"

using klotski::cases::AllCases;

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

void AllCases::build_cases(const int head, Ranges &release) {
    release.clear();
    release.reserve(ALL_CASES_NUM[head]);
    BasicRanges::instance().fetch().with_head(head, release);
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
