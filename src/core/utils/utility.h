#pragma once

#include <bit>
#include <list>
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

/// Get the number of consecutive `0` in the low bits.
// inline int low_zero_num(const uint32_t bin) {
//     return __builtin_ctzl(bin);
//
//     // TODO: using (bin ^ (bin - 1)) when non-builtin
//
//     // WARN: be aware of serious performance issues
//     // return __builtin_popcount(~(bin ^ -bin)) - 1;
// }

/// Get the number of consecutive `0` in the low bits.
// inline int low_zero_num(const uint64_t bin) {
//     return __builtin_ctzll(bin);
//
//     // WARN: be aware of serious performance issues
//     // return __builtin_popcount(~(bin ^ -bin)) - 1;
// }

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
    using After = std::function<void(Executor &&)>;

    /// Construction based on executor.
    explicit Worker(Executor &&executor);

    /// Post new task into the queue.
    void post(Task &&task);

    /// Setting up callback entry.
    void then(After &&after);

    /// Tasks will be triggered at destruction.
    ~Worker();

private:
    Notifier after_;
    Executor executor_;
    std::list<Task> tasks_;
};

} // namespace klotski

#include "worker.inl"
