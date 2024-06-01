#pragma once

#include <bit>
#include <list>
#include <numeric>
#include <functional>

/// Mark target class as a singleton.
#define KLSK_INSTANCE(T)                  \
    private:                              \
        T() = default;                    \
    public:                               \
        T(T &&) = delete;                 \
        T(const T &) = delete;            \
        T& operator=(T &&) = delete;      \
        T& operator=(const T &) = delete; \
        static T& instance() {            \
            static T ins;                 \
            return ins;                   \
        }

/// Marking compiler assumptions.
#define KLSK_ASSUME(expr) __builtin_assume(expr)

/// Force function declaration to be inline.
#define KLSK_INLINE __attribute__((always_inline))

namespace klotski {

template <typename T>
concept Addable = requires(T a, T b) { a + b; };

template <Addable T, size_t N>
consteval int array_sum(const std::array<T, N> &arr) {
    return std::accumulate(arr.begin(), arr.end(), 0);
}

template <Addable T, size_t N>
consteval std::array<T, N> to_offset(const std::array<T, N> &arr, T base) {

    static_assert(N > 0);

    std::array<T, N> offset;

    T val = base;

    offset[0] = 0;

    for (int i = 0; i < N - 1; ++i) {
        val += arr[i];
        offset[i + 1] = val;
    }

    return offset;

}

/// Flips the input u32 every two bits in low-high symmetry.
inline uint32_t range_reverse(uint32_t bin) {
    bin = std::byteswap(bin);
// #if defined(__GNUC__) || defined(__clang__)
//     bin = __builtin_bswap32(bin);
// #else
//     // FIXME: `_byteswap_ulong` under MSVC
//     // TODO: using `std::byteswap` (c++23)
//     bin = ((bin << 16) & 0xFFFF0000) | ((bin >> 16) & 0x0000FFFF);
//     bin = ((bin << 8) & 0xFF00FF00) | ((bin >> 8) & 0x00FF00FF);
// #endif
    bin = ((bin << 4) & 0xF0F0F0F0) | ((bin >> 4) & 0x0F0F0F0F);
    return ((bin << 2) & 0xCCCCCCCC) | ((bin >> 2) & 0x33333333);
}

/// Empty function calls that generally used for callbacks.
typedef std::function<void()> Notifier;

/// Execute the passed function that generally used for concurrency.
typedef std::function<void(std::function<void()> &&)> Executor;

/// Perform multiple tasks without blocking and trigger callback upon completion.
class Worker final {
public:
    using Task = std::function<void()>;

    /// Construction based on executor.
    explicit Worker(Executor executor);

    /// Post new task into the queue.
    void post(Task &&task);

    /// Setting up callback entry.
    void then(Notifier &&after);

    /// Tasks will be triggered at destruction.
    ~Worker();

private:
    Notifier after_;
    Executor executor_;
    std::list<Task> tasks_;
};

} // namespace klotski

#include "worker.inl"
