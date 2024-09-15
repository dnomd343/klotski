#pragma once

#include <span>
#include <functional>

#include "raw_code/raw_code.h"
#include "all_cases/all_cases.h"
#include "short_code/short_code.h"
#include "common_code/common_code.h"

using klotski::codec::RawCode;
using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

/// Build all valid CommonCodes.
std::vector<uint64_t> all_common_codes();

// ----------------------------------------------------------------------------------------- //

/// Capture ostream output as string.
template <typename T>
std::string ostream_capture(T obj) {
    std::ostringstream out;
    out << obj; // ostream capture
    return out.str();
}

#define EXPECT_OSTREAM(obj, expect) EXPECT_EQ(ostream_capture(obj), expect)

// ----------------------------------------------------------------------------------------- //

/// Spawn all valid RawCodes in parallel.
void raw_code_parallel(std::function<void(std::span<RawCode>)> &&func);

/// Spawn all valid ShortCodes in parallel.
void short_code_parallel(std::function<void(std::span<ShortCode>)> &&func);

/// Spawn all valid CommonCodes in parallel.
void common_code_parallel(std::function<void(std::span<CommonCode>)> &&func);

#define CODE_PARALLEL(Type, type, impl)                    \
    type##_code_parallel([](std::span<Type##Code> codes) { \
        for (auto code : codes) {impl}                     \
    })

#define RAW_CODE_PARALLEL(impl) CODE_PARALLEL(Raw, raw, impl)
#define SHORT_CODE_PARALLEL(impl) CODE_PARALLEL(Short, short, impl)
#define COMMON_CODE_PARALLEL(impl) CODE_PARALLEL(Common, common, impl)

// ----------------------------------------------------------------------------------------- //

/// Calculate multiple ranges separately and combine the results.
template<typename T, typename F>
requires std::is_integral_v<T> && std::is_invocable_v<F, T, T>
auto parallel_spawn(T limit, F &&func) -> std::invoke_result_t<F, T, T> {
    BS::thread_pool pool;
    std::invoke_result_t<F, T, T> result;
    for (auto &&future : pool.submit_blocks((T)0, limit, func, 0x1000)) {
        const auto data = future.get();
        result.insert(result.end(), std::begin(data), std::end(data)); // combine sections
    }
    pool.wait();
    return result;
}

// ----------------------------------------------------------------------------------------- //
