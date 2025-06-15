#include <benchmark/benchmark.h>

#include "analyse/analyse.h"
#include "common_code/common_code.h"

using klotski::Analyse;
using klotski::analyse::AnalysePro;

static void AnalyseBenchmark(benchmark::State &state) {

    auto code = klotski::codec::CommonCode::unsafe_create(0x1A9BF0C00).to_raw_code();

    for (auto _ : state) {

        // auto analyse = Analyse {code};
        // analyse.build();

        auto analyse = AnalysePro {code};
        analyse.build_all();

    }

}

BENCHMARK(AnalyseBenchmark)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
