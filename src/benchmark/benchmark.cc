#include "benchmark.h"

#include "basic_ranges.h"

const char split_line[] = "------------------------";



void Benchmark::basic_ranges(std::ostream &out) {
    out << split_line << std::endl;
    out << "Basic Ranges Benchmark" << std::endl;
    if (BasicRanges::status() != BasicRanges::NO_INIT) {
        out << "already built -> skip" << std::endl;
        out << split_line << std::endl;
        return;
    }
    auto start = clock();
    BasicRanges::build();
//    time_ms(start, out);
//    out << time_ms_(start);
    out << time_ms_str(start);
    out << std::endl << split_line << std::endl;
}

long Benchmark::time_ms_(clock_t start) {
    return (clock() - start) * 1000 / CLOCKS_PER_SEC;
}

void Benchmark::time_ms(clock_t start, std::ostream &out) {
    out << (clock() - start) * 1000 / CLOCKS_PER_SEC << "ms";
}

void Benchmark::time_us(clock_t start, std::ostream &out) {
    out << (clock() - start) * 1000000 / CLOCKS_PER_SEC << "us";
}

std::string Benchmark::time_ms_str(clock_t start) {
    return std::string("\033[32m") + std::to_string((clock() - start) * 1000 / CLOCKS_PER_SEC) + "ms\033[0m";
}
