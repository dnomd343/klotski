#include <thread>
#include "benchmark.h"

using klotski::Benchmark;

uint32_t Benchmark::core_num() noexcept {
    return std::thread::hardware_concurrency();
}

double Benchmark::warm_up(uint64_t count) noexcept {
    auto start = clock();
    volatile uint64_t tmp = 0;
    for (; tmp < count; ++tmp);
    return time_format(start, US);
}

double Benchmark::time_format(clock_t start, TIME format) noexcept {
    auto time = double(clock() - start);
    switch (format) {
        case S:
            time *= double(1);
            break;
        case MS:
            time *= double(1000);
            break;
        case US:
            time *= double(1000 * 1000);
            break;
        case NS:
            time *= double(1000 * 1000 * 1000);
            break;
    }
    return time / CLOCKS_PER_SEC;
}
