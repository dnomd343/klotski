#include <benchmark/benchmark.h>

#include <iostream>

#include "utils/utility.h"
#include "all_cases/all_cases.h"

using klotski::cases::Ranges;
using klotski::cases::BasicRanges;

static Ranges range_samples() {
    auto ranges = BasicRanges::instance().fetch();

    Ranges samples;

    for (uint64_t i = 0; i < ranges.size(); ++i) {
        if (i % 73500 == 0) {
            samples.emplace_back(ranges[i]);
        }
    }

    return samples;
}

static void range_reverse(benchmark::State &state) {

    auto samples = range_samples();

    for (auto _ : state) {
        for (const auto range : samples) {
            volatile auto kk = klotski::range_reverse(range);
        }
    }

}

BENCHMARK(range_reverse);

BENCHMARK_MAIN();
