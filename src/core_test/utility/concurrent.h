#pragma once

/// The concurrency test helper class implements the Executor and Racer based
/// on the BS::thread_pool. The Executor allows you to submit multiple tasks
/// and dispatch them to different threads. The Racer schedules the specified
/// function to a certain number of threads at one time.

#include <memory>
#include <functional>

#include "BS_thread_pool.hpp"

namespace co {

class Executor final {
public:
    Executor() = default;
    ~Executor() { pool_.wait(); }

    std::function<void(std::function<void()> &&)> Entry() {
        return [this](auto &&func) {
            pool_.detach_task(func);
        };
    }

private:
    BS::thread_pool pool_;
};

class Racer final {
public:
    Racer() = default;
    ~Racer() { Join(); }

    static constexpr int Num = 256; // number of racing threads

    void Begin(std::function<void()> &&item) {
        item_ = std::move(item);
        pool_.detach_sequence(0, Num, [this](const int) {
            item_(); // execute racing function
        });
    }

    void Join() { pool_.wait(); }

private:
    std::function<void()> item_;
    BS::thread_pool pool_ { Num };
};


} // namespace co
