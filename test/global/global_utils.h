#include <future>
#include <vector>
#include <functional>
#include "tiny_pool.h"

class TinyPool { // OOP for tiny_thread_pool
    pool_t *pool;
    static void wrap_c_func(void *func) { // wrap std::function as c-style function ptr
        auto func_ptr = static_cast<std::function<void()>*>(func);
        (*func_ptr)();
        delete func_ptr;
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

typedef std::vector<std::pair<uint64_t, uint64_t>> range_split_t;

range_split_t range_split(uint64_t start, uint64_t end, uint64_t size);
