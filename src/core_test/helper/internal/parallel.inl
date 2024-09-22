#pragma once

#include <BS_thread_pool.hpp>

namespace helper {

template<typename T, typename F>
requires std::is_integral_v<T> && std::is_invocable_v<F, T, T>
auto scope_parallel(T limit, F &&func) -> std::invoke_result_t<F, T, T> {
    BS::thread_pool pool;
    std::invoke_result_t<F, T, T> result;
    for (auto &&future : pool.submit_blocks((T)0, limit, func, 0x1000)) {
        const auto data = future.get();
        result.insert(result.end(), std::begin(data), std::end(data)); // combine sections
    }
    pool.wait();
    return result;
}

} // namespace helper
