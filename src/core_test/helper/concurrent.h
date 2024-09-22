#pragma once

/// The concurrency test helper class implements the Executor and Racer based
/// on the BS::thread_pool. The Executor allows you to submit multiple tasks
/// and dispatch them to different threads. The Racer schedules the specified
/// function to a certain number of threads at one time.

#include <functional>
#include <BS_thread_pool.hpp>

namespace helper {

// ----------------------------------------------------------------------------------------- //

class Executor final {
public:
    /// Specify the number of threads, default to the number of CPU cores.
    explicit Executor(const int num = 0) : pool_(num) {}

    /// Get the executor entry for submitting tasks.
    std::function<void(std::function<void()> &&func)> Entry();

    ~Executor();

private:
    BS::thread_pool pool_;
};

// ----------------------------------------------------------------------------------------- //

class Racer final {
public:
    /// Create race tester with a specified number of concurrency.
    explicit Racer(const int num = 256) : race_num_(num), pool_(num) {}

    /// Start the racing test process with specified number.
    void Execute(std::function<void()> &&item);

    /// Get the number of concurrency in Racer.
    int RaceNum() const;

    ~Racer();

private:
    const int race_num_;
    BS::thread_pool pool_;
};

// ----------------------------------------------------------------------------------------- //

} // namespace helper
