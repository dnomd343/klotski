#pragma once

#include <future>
#include <vector>
#include <functional>
#include "tiny_pool.h"

uint32_t thread_num();

typedef std::vector<std::pair<uint64_t, uint64_t>> range_split_t;
range_split_t range_split(uint64_t start, uint64_t end, uint64_t size);

class TinyPool { // OOP for tiny_thread_pool
    pool_t *pool;
    static void wrap_c_func(void *func) { // wrap std::function as c-style function ptr
        auto func_ptr = static_cast<std::function<void()>*>(func);
        (*func_ptr)();
        delete func_ptr; // free lambda function
    }

public:
    void boot() { tiny_pool_boot(pool); }
    void join() { tiny_pool_join(pool); }
    void kill() { tiny_pool_kill(pool); }
    void detach() { tiny_pool_detach(pool); }
    explicit TinyPool(uint32_t size) { pool = tiny_pool_create(size); }
    // TODO: thread pool destroy

    template <typename Func, typename ...Args>
    auto submit(Func &&func, Args &&...args) -> std::future<decltype(func(args...))> { // submit new task
        std::function<decltype(func(args...))()> wrap_func = std::bind(
            std::forward<Func>(func), std::forward<Args>(args)... // wrap as a function without params
        );
        auto func_ptr = std::make_shared<
            std::packaged_task<decltype(func(args...))()> // function task as shared ptr
        >(wrap_func);
        tiny_pool_submit(pool, TinyPool::wrap_c_func, (void*)( // submit with thread pool interface
            new std::function<void()> (
                [func_ptr]() { (*func_ptr)(); } // create lambda for running task
            )
        ));
        return func_ptr->get_future(); // return future object
    }
};
