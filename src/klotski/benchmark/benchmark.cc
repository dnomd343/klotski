#include "benchmark.h"
#include "all_cases.h"

using namespace klotski;

float Benchmark::basic_ranges(Benchmark::TIME format) {
    if (BasicRanges::status() != BasicRanges::NO_INIT) {
        return -1; // data already built -> skip
    }
    auto start = clock();
    BasicRanges::build();
    return time_format(start, format);
}

float Benchmark::all_cases(Benchmark::TIME format) {
    if (AllCases::status() != AllCases::NO_INIT) {
        return -1; // data already built -> skip
    }
    BasicRanges::build(); // preparing benchmark data
    auto start = clock();
    AllCases::build();
    return time_format(start, format);
}
