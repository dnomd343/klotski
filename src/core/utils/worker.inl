#pragma once

#include <atomic>
#include <memory>

namespace klotski {

inline Worker::Worker(Executor executor)
    : after_([] {}), executor_(std::move(executor)) {}

inline void Worker::post(Task &&task) {
    tasks_.emplace_back(std::move(task));
}

inline void Worker::then(Notifier &&after) {
    after_ = std::move(after);
}

inline Worker::~Worker() {
    if (tasks_.empty()) {
        executor_([after = std::move(after_)] {
            after(); // callback directly
        });
        return;
    }

    auto counter = std::make_shared<std::atomic_size_t>(tasks_.size());
    for (auto &&task : tasks_) {
        executor_([counter, after = after_, task = std::move(task)] {
            task();
            if (counter->fetch_sub(1) == 1) {
                after(); // all tasks done
            }
        });
    }
}

} // namespace klotski
