#include "benchmark.h"

using klotski::Benchmark;

float Benchmark::time_format(clock_t start, TIME format) {
    auto time = float(clock() - start);
    switch (format) {
        case S:
            time *= float(1);
            break;
        case MS:
            time *= float(1000);
            break;
        case US:
            time *= float(1000 * 1000);
            break;
        case NS:
            time *= float(1000 * 1000 * 1000);
            break;
    }
    return time / CLOCKS_PER_SEC;
}
