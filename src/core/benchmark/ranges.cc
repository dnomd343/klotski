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
using klotski::cases::BASIC_RANGES_NUM_;

Ranges ranges_samples(const size_t num) {
    auto &ranges = BasicRanges::instance().fetch();
    const size_t part_size = ranges.size() / num;
    const size_t offset = part_size / 2;

    Ranges result;
    for (size_t i = 0; i < num; ++i) {
        result.emplace_back(ranges[i * part_size + offset]);
    }
    return result;
}

static void CheckRanges(benchmark::State &state) {
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

static void ReverseRanges(benchmark::State &state) {
    auto ranges = BasicRanges::instance().fetch();
    for (auto _ : state) {
        ranges.reverse();
    }
}

static void SpawnRanges(benchmark::State &state) {
    for (auto _ : state) {
        Ranges ranges {};
        ranges.reserve(BASIC_RANGES_NUM_);
        for (const auto [n, n_2x1, n_1x1] : BLOCK_NUM) {
            ranges.spawn(n, n_2x1, n_1x1);
        }
    }
}

static void DeriveRanges(benchmark::State &state) {
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

static void RangesUnionExport(benchmark::State &state) {
    auto &all_cases = AllCases::instance().fetch();
    for (auto _ : state) {
        auto codes = all_cases.codes();
        benchmark::DoNotOptimize(codes.size());
    }
}

static void RangesSize(benchmark::State &state) {
    auto &all_cases = AllCases::instance().fetch();
    // std::cout << all_cases.size() << std::endl;
    for (auto _ : state) {
        volatile auto k1 = all_cases.size();
        volatile auto k2 = all_cases.size();
        volatile auto k3 = all_cases.size();
        volatile auto k4 = all_cases.size();
        volatile auto k5 = all_cases.size();
        volatile auto k6 = all_cases.size();
        volatile auto k7 = all_cases.size();
        volatile auto k8 = all_cases.size();

        volatile auto p1 = all_cases.size();
        volatile auto p2 = all_cases.size();
        volatile auto p3 = all_cases.size();
        volatile auto p4 = all_cases.size();
        volatile auto p5 = all_cases.size();
        volatile auto p6 = all_cases.size();
        volatile auto p7 = all_cases.size();
        volatile auto p8 = all_cases.size();
    }
}

static void RangesAt(benchmark::State &state) {
    auto &all_cases = AllCases::instance().fetch();
    for (auto _ : state) {
        volatile auto k0 = all_cases[1035968];
        volatile auto k1 = all_cases[3778871];
        volatile auto k2 = all_cases[7489354];
        volatile auto k3 = all_cases[10398492];
        volatile auto k4 = all_cases[19091276];
        volatile auto k5 = all_cases[21373726];
        volatile auto k6 = all_cases[27296711];
        volatile auto k7 = all_cases[28214648];
    }
}

BENCHMARK(CheckRanges)->Unit(benchmark::kMicrosecond)->Range(16, 256);

// BENCHMARK(ReverseRanges)->Unit(benchmark::kMillisecond);

// BENCHMARK(SpawnRanges)->Unit(benchmark::kMillisecond);

// BENCHMARK(DeriveRanges)->Unit(benchmark::kMillisecond);

// BENCHMARK(RangesUnionExport)->Unit(benchmark::kMillisecond);

// BENCHMARK(RangesSize);

// BENCHMARK(RangesAt);

BENCHMARK_MAIN();
