#include <benchmark/benchmark.h>

#include "group/group.h"
#include "ranges/ranges.h"
#include "all_cases/all_cases.h"

using klotski::cases::Ranges;
using klotski::cases::AllCases;
using klotski::cases::BasicRanges;
using klotski::cases::RangesUnion;

using klotski::group::BLOCK_NUM;
using klotski::cases::ALL_CASES_NUM;
using klotski::cases::ALL_CASES_NUM_;
using klotski::cases::BASIC_RANGES_NUM_;

Ranges ranges_samples(const size_t num) {
    auto &ranges = BasicRanges::instance().fetch();
    const size_t part_size = ranges.size() / num;
    const size_t offset = part_size / 2;

    Ranges result;
    result.reserve(num);
    for (size_t i = 0; i < num; ++i) {
        result.emplace_back(ranges[i * part_size + offset]);
    }
    return result;
}

std::vector<size_t> all_cases_index_samples(const size_t num) {
    const size_t part_size = ALL_CASES_NUM_ / num;
    const size_t offset = part_size / 2;

    std::vector<size_t> result;
    result.reserve(num);
    for (size_t i = 0; i < num; ++i) {
        result.emplace_back(i * part_size + offset);
    }
    return result;
}

static void RangesReverse(benchmark::State &state) {
    auto ranges = BasicRanges::instance().fetch();
    for (auto _ : state) {
        ranges.reverse();
    }
}

static void RangesCheck(benchmark::State &state) {
    auto ranges = ranges_samples(state.range(0));
    ranges.reverse();
    for (auto _ : state) {
        for (const auto range : ranges) {
            for (const auto head : RangesUnion::Heads) {
                benchmark::DoNotOptimize(Ranges::check(static_cast<int>(head), range));
            }
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0) * 12);
}

static void RangesSpawn(benchmark::State &state) {
    for (auto _ : state) {
        Ranges ranges {};
        ranges.reserve(BASIC_RANGES_NUM_);
        for (const auto [n, n_2x1, n_1x1] : BLOCK_NUM) {
            ranges.spawn(n, n_2x1, n_1x1);
        }
    }
}

static void RangesDerive(benchmark::State &state) {
    auto ranges = BasicRanges::instance().fetch();
    ranges.reverse();
    for (auto _ : state) {
        RangesUnion cases;
        for (const auto head : RangesUnion::Heads) {
            cases.ranges(head).reserve(ALL_CASES_NUM[head]);
            ranges.derive(static_cast<int>(head), cases.ranges(head));
        }
    }
}

static void RangesUnionAt(benchmark::State &state) {
    const auto &all_cases = AllCases::instance().fetch();
    const auto samples = all_cases_index_samples(state.range(0));
    for (auto _ : state) {
        for (const auto index : samples) {
            benchmark::DoNotOptimize(all_cases[index]);
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void RangesUnionSize(benchmark::State &state) {
    auto &all_cases = AllCases::instance().fetch();
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            benchmark::DoNotOptimize(all_cases.size());
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void RangesUnionExport(benchmark::State &state) {
    auto &all_cases = AllCases::instance().fetch();
    for (auto _ : state) {
        auto codes = all_cases.codes();
        benchmark::DoNotOptimize(codes.size());
    }
}

BENCHMARK(RangesReverse)->Unit(benchmark::kMillisecond);
BENCHMARK(RangesCheck)->Unit(benchmark::kMicrosecond)->Range(16, 256);

BENCHMARK(RangesSpawn)->Unit(benchmark::kMillisecond);
BENCHMARK(RangesDerive)->Unit(benchmark::kMillisecond);

BENCHMARK(RangesUnionAt)->Range(16, 256);
BENCHMARK(RangesUnionSize)->Range(16, 256);
BENCHMARK(RangesUnionExport)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
