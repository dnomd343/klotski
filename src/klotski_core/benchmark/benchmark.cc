#include "benchmark.h"
#include "all_cases.h"

using klotski::Benchmark;

float Benchmark::basic_ranges(TIME format) {
    if (BasicRanges::status() != BasicRanges::NO_INIT) {
        return -1; // data already built -> skip
    }
    auto start = clock();
    BasicRanges::build();
    return time_format(start, format);
}

float Benchmark::all_cases(TIME format) {
    if (AllCases::status() != AllCases::NO_INIT) {
        return -1; // data already built -> skip
    }
    BasicRanges::build(); // preparing benchmark data
    auto start = clock();
    AllCases::build();
    return time_format(start, format);
}
