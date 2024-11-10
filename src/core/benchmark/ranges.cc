#include <benchmark/benchmark.h>

#include "group/group.h"
#include "ranges/ranges.h"
#include "all_cases/all_cases.h"

using klotski::cases::AllCases;

static void SpawnRanges(benchmark::State &state) {
    // constexpr auto nums = target_nums();

    for (auto _ : state) {

        klotski::cases::Ranges kk {};
        kk.reserve(7311921);

        // for (uint32_t type_id = 0; type_id < klotski::cases::TYPE_ID_LIMIT; ++type_id) {
            for (auto [n, n_2x1, n_1x1] : klotski::group::BLOCK_NUM) {
                kk.spawn(n, n_2x1, n_1x1);
            }
        // }
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

// BENCHMARK(SpawnRanges)->Unit(benchmark::kMillisecond);

// BENCHMARK(RangesUnionExport)->Unit(benchmark::kMillisecond);

// BENCHMARK(RangesSize);

BENCHMARK(RangesAt);

BENCHMARK_MAIN();
