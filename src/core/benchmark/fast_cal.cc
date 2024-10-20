#include <benchmark/benchmark.h>

#include "fast_cal/fast_cal.h"

#include "raw_code/raw_code.h"

using klotski::codec::CommonCode;

static void FastCalBenchmark(benchmark::State &state) {

    auto code = CommonCode::unsafe_create(0x1A9BF0C00).to_raw_code();

    for (auto _ : state) {
        // auto fc = FastCal(code);
        // benchmark::DoNotOptimize(fc.solve());
        benchmark::DoNotOptimize(FastCal_demo(code));
    }

}

BENCHMARK(FastCalBenchmark)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
