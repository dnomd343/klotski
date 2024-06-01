#pragma once

/// The concurrency test helper class implements the Executor and Racer based
/// on the BS::thread_pool. The Executor allows you to submit multiple tasks
/// and dispatch them to different threads. The Racer schedules the specified
/// function to a certain number of threads at one time.

#include <functional>

#include "BS_thread_pool.hpp"

namespace co {

class Executor final {
public:
    explicit Executor(const int num) : pool_(num) {}

    std::function<void(std::function<void()> &&)> Entry() {
        return [this](auto &&func) {
            pool_.detach_task(func);
        };
    }

    ~Executor() {
        pool_.wait();
    }

private:
    BS::thread_pool pool_;
};

class Racer final {
public:
    explicit Racer(const int num) : race_num_(num), pool_(num) {}

    int RaceNum() const {
        return race_num_; // number of racing threads
    }

    void Start(std::function<void()> &&item) {
        auto wrapper = [item = std::move(item)](const int) {
            item(); // execute racing function
        };
        pool_.detach_sequence(0, race_num_, wrapper);
    }

    void Join() {
        pool_.wait();
    }

    ~Racer() {
        Join();
    }

private:
    const int race_num_;
    BS::thread_pool pool_;
};

} // namespace co
