#include <future>
#include <functional>
#include "tiny_pool.h"

class TinyPool { // OOP for tiny_thread_pool
    pool_t *pool;
    static void wrap_c_func(void *func) { // wrap std::function as c-style function ptr
        (*static_cast<std::function<void()>*>(func))();
        free(func);
    }

public:
    void boot() { tiny_pool_boot(pool); }
    void join() { tiny_pool_join(pool); }
    void kill() { tiny_pool_kill(pool); }
    void detach() { tiny_pool_detach(pool); }
    explicit TinyPool(uint32_t size) { pool = tiny_pool_create(size); }

    template <typename Func, typename ...Args>
    auto submit(Func &&func, Args &&...args) -> std::future<decltype(func(args...))>;
};

template <typename Func, typename ...Args>
auto TinyPool::submit(Func &&func, Args &&...args) -> std::future<decltype(func(args...))> {
    std::function<decltype(func(args...))()> wrap_func = std::bind(
        std::forward<Func>(func), std::forward<Args>(args)...
    );
    auto func_ptr = std::make_shared<
        std::packaged_task<decltype(func(args...))()>
    >(wrap_func);
    tiny_pool_submit(pool, TinyPool::wrap_c_func, (void*)(
        new std::function<void()> (
            [func_ptr]() { (*func_ptr)(); }
        )
    ));
    return func_ptr->get_future();
}
