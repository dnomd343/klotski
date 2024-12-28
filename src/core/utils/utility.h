#pragma once

#include <bit>
#include <list>
#include <utility>
#include <cstdint>
#include <numeric>
#include <functional>
#include <type_traits>

// ----------------------------------------------------------------------------------------- //

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
#if defined(__clang__)
  #define KLSK_ASSUME(expr) __builtin_assume(expr)
#elif defined(__GNUC__)
  #define KLSK_ASSUME(expr) [[assume(expr)]]
#endif

#define KLSK_UNREACHABLE __builtin_unreachable() // TODO: using `std::unreachable`

/// Force function declaration to be inline.
#if defined(__clang__)
  #define KLSK_INLINE __attribute__ ((always_inline))
#else
  #define KLSK_INLINE // NOTE: make sure that function can be inline
#endif
#define KLSK_INLINE_H KLSK_INLINE inline
#define KLSK_INLINE_CE KLSK_INLINE constexpr

#define KLSK_STRING(x) #x

#if defined(__clang__)
  #define KLSK_UNROLL(N) _Pragma(KLSK_STRING(unroll N))
#elif defined(__GNUC__)
  #define KLSK_UNROLL(N) _Pragma(KLSK_STRING(GCC unroll N))
#endif

#if defined(__clang__)
  #define KLSK_IVDEP _Pragma("clang loop vectorize(enable)")
#elif defined(__GNUC__)
  #define KLSK_IVDEP _Pragma("GCC ivdep")
#endif

/// Prevent reordering for both compiler and processor.
#define KLSK_MEM_BARRIER std::atomic_thread_fence(std::memory_order_seq_cst)

// ----------------------------------------------------------------------------------------- //

namespace klotski {

template <typename T>
#ifndef __cpp_lib_has_unique_object_representations
concept is_compact_layout_v = true;
#else
concept is_compact_layout_v = std::has_unique_object_representations_v<T>;
#endif

/// Calculate the sum of an array of integers.
template <typename T, size_t N>
requires std::is_integral_v<T>
consteval int array_sum(const std::array<T, N> &arr) {
    return std::accumulate(arr.begin(), arr.end(), 0);
}

/// Calculate offset list of integer array with sizes.
template <typename T, size_t N>
requires std::is_integral_v<T> && (N > 0)
consteval std::array<T, N> to_offset(const std::array<T, N> &arr) {
    std::array<T, N> offset {};
    std::partial_sum(arr.begin(), arr.end() - 1, offset.begin() + 1);
    return offset;
}

/// Flips the input u32 every two bits in low-high symmetry.
constexpr uint32_t range_reverse(uint32_t bin) {
    bin = std::byteswap(bin);
    bin = ((bin << 4) & 0xF0F0F0F0) | ((bin >> 4) & 0x0F0F0F0F);
    return ((bin << 2) & 0xCCCCCCCC) | ((bin >> 2) & 0x33333333);
}

// ----------------------------------------------------------------------------------------- //

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

// ----------------------------------------------------------------------------------------- //

} // namespace klotski

#include "worker.inl"
