#include "benchmark.h"
#include "all_cases.h"

const char line[] = "------------------------";

void Benchmark::basic_ranges(std::ostream &out) {
    out << line << std::endl;
    out << "Basic Ranges Benchmark" << std::endl;
    if (BasicRanges::status() != BasicRanges::NO_INIT) {
        out << "already built -> " << color_red("skip") << std::endl;
        out << line << std::endl;
        return;
    }
    /// start benchmark process
    auto start = clock();
    BasicRanges::build();
    out << "time -> " << time_ms(start) << std::endl;
    out << line << std::endl;
}

void Benchmark::all_cases(std::ostream &out) {
    out << line << std::endl;
    out << "All Cases Benchmark" << std::endl;
    if (AllCases::status() != AllCases::NO_INIT) {
        out << "already built -> " << color_red("skip") << std::endl;
        out << line << std::endl;
        return;
    }
    /// preparing benchmark data
    BasicRanges::build();
    /// start benchmark process
    auto start = clock();
    AllCases::build();
    out << "time -> " << time_ms(start) << std::endl;
    out << line << std::endl;
}
