#pragma once

#include <memory>
#include <functional>

#include "BS_thread_pool.hpp"

namespace co {

class Racer final {
public:
    static constexpr int Times = 256;

    explicit Racer(std::function<void()> &&item) : pool_(Times), item_(item) {
        pool_.detach_sequence(0, Times, [this](const int) {
            item_(); // execute race function
        });
    }

    ~Racer() { Join(); }

    void Join() { pool_.wait(); }

private:
    BS::thread_pool pool_;
    std::function<void()> item_;
};

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

} // namespace co
