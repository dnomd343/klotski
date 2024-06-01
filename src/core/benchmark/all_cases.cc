#include <BS_thread_pool.hpp>
#include <benchmark/benchmark.h>

#include "all_cases/all_cases.h"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

static void BM_BasicRanges(benchmark::State &state) {
    auto &basic_ranges = BasicRanges::instance();
    for (auto _ : state) {
        basic_ranges.available_ = false;
        basic_ranges.build();
    }
}

static void BM_BasicRangesAsync(benchmark::State &state) {
    const uint32_t thread_num = state.range(0);
    BS::thread_pool pool {thread_num};
    auto executor = [&pool](auto &&func) {
        pool.detach_task(func);
    };

    auto &basic_ranges = BasicRanges::instance();
    for (auto _ : state) {
        basic_ranges.available_ = false;
        basic_ranges.build_async(executor, [] {});
        pool.wait();
    }
}

static void BM_AllCases(benchmark::State &state) {
    BasicRanges::instance().build();
    auto &all_cases = AllCases::instance();
    for (auto _ : state) {
        all_cases.available_ = false;
        all_cases.build();
    }
}

static void BM_AllCasesAsync(benchmark::State &state) {
    const uint32_t thread_num = state.range(0);
    BS::thread_pool pool {thread_num};
    auto executor = [&pool](auto &&func) {
        pool.detach_task(func);
    };

    BasicRanges::instance().build();
    auto &all_cases = AllCases::instance();
    for (auto _ : state) {
        all_cases.available_ = false;
        all_cases.build_async(executor, [] {});
        pool.wait();
    }
}

BENCHMARK(BM_BasicRanges)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_BasicRangesAsync)->Unit(benchmark::kMillisecond)->UseRealTime()->Arg(1)->DenseRange(2, 16, 2);

BENCHMARK(BM_AllCases)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_AllCasesAsync)->Unit(benchmark::kMillisecond)->UseRealTime()->Arg(1)->DenseRange(2, 16, 2);
