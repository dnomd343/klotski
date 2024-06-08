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
            for (auto [n, n_2x1, n_1x1] : klotski::cases::BLOCK_NUM) {
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

BENCHMARK(SpawnRanges)->Unit(benchmark::kMillisecond);

// BENCHMARK(RangesUnionExport)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
