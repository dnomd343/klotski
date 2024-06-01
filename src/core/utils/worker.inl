#pragma once

#include <atomic>

namespace klotski {

inline Worker::Worker(Executor &&executor)
    : after_([] {}), executor_(executor) {}

inline void Worker::post(Task &&task) {
    tasks_.emplace_back(std::move(task));
}

inline void Worker::then(After &&after) {
    after_ = [after = std::move(after), executor = executor_]() mutable {
        after(std::move(executor));
    };
}

inline Worker::~Worker() {
    if (tasks_.empty()) {
        executor_([after = after_] {
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
