#include "global_utils.h"

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

range_split_t range_split(uint64_t start, uint64_t end, uint64_t size) {
    uint64_t rear = end - (end - start) % size; // (rear - start) % size == 0
    range_split_t ranges;
    for (uint64_t i = start; i < rear; i += size) {
        ranges.emplace_back(i, i + size);
    }
    if (rear != end) {
        ranges.emplace_back(rear, end);
    }
    return ranges;
}
