#include <benchmark/benchmark.h>

#include "fast_cal/fast_cal.h"

#include "raw_code/raw_code.h"

#include "group/group.h"

using klotski::codec::CommonCode;

using klotski::fast_cal::FastCalPro;

static void FastCalBenchmark(benchmark::State &state) {

    auto code = CommonCode::unsafe_create(0x1A9BF0C00).to_raw_code();
//    auto code = CommonCode::unsafe_create(0x4FEA13400).to_raw_code();

    for (auto _ : state) {
        auto fc = FastCalPro(code);
        benchmark::DoNotOptimize(fc.solve());
//        benchmark::DoNotOptimize(fc.demo());
        // benchmark::DoNotOptimize(FastCal_demo(code));
//        auto tmp = klotski::cases::Group_extend(code);
    }

}

BENCHMARK(FastCalBenchmark)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
