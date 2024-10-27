#include <benchmark/benchmark.h>

#include "fast_cal/fast_cal.h"

#include "raw_code/raw_code.h"

#include "group/group.h"

using klotski::codec::CommonCode;

using klotski::fast_cal::FastCal;

static void FastCalBenchmark(benchmark::State &state) {

    auto code = CommonCode::unsafe_create(0x1A9BF0C00).to_raw_code();
    // auto code = CommonCode::unsafe_create(0x4FEA13400).to_raw_code();

    for (auto _ : state) {
        auto fc = FastCal(code);

        benchmark::DoNotOptimize(fc.solve());
        // benchmark::DoNotOptimize(fc.solve_multi());
        // benchmark::DoNotOptimize(fc.furthest());
        // fc.search([](const klotski::codec::RawCode tmp) {
        //     return tmp == 0x7F87E0E5BFFF492;
        //     // return tmp == 0x1FB1E37F9E9F5CA;
        // });
        // fc.build_all();
    }

}

BENCHMARK(FastCalBenchmark)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
