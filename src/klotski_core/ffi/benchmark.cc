#include "klotski.h"
#include "benchmark.h"

using klotski::Benchmark;

///////////////////////////////////////////////////////////////////////////////

void benchmark_preparation() {
    Benchmark::data_preparation();
}

double benchmark_warm_up(uint64_t count) {
    return Benchmark::warm_up(count);
}

///////////////////////////////////////////////////////////////////////////////

double benchmark_range_flip_ns() {
    return Benchmark::range_flip(Benchmark::NS);
}

///////////////////////////////////////////////////////////////////////////////

double benchmark_all_cases_ms() {
    return Benchmark::all_cases(Benchmark::MS);
}
double benchmark_basic_ranges_ms() {
    return Benchmark::basic_ranges(Benchmark::MS);
}

///////////////////////////////////////////////////////////////////////////////

double benchmark_raw_code_check_ns() {
    return Benchmark::raw_code_check(Benchmark::NS);
}
double benchmark_short_code_check_ns() {
    return Benchmark::short_code_check(Benchmark::NS);
}
double benchmark_common_code_check_ns() {
    return Benchmark::common_code_check(Benchmark::NS);
}
double benchmark_raw_code_check_random_ns() {
    return Benchmark::raw_code_check_random(Benchmark::NS);
}
double benchmark_short_code_check_random_ns() {
    return Benchmark::short_code_check_random(Benchmark::NS);
}
double benchmark_common_code_check_random_ns() {
    return Benchmark::common_code_check_random(Benchmark::NS);
}

///////////////////////////////////////////////////////////////////////////////

double benchmark_short_code_to_string_ns() {
    return Benchmark::short_code_to_string(Benchmark::NS);
}
double benchmark_short_code_from_string_ns() {
    return Benchmark::short_code_from_string(Benchmark::NS);
}
double benchmark_common_code_to_string_ns() {
    return Benchmark::common_code_to_string(Benchmark::NS);
}
double benchmark_common_code_from_string_ns() {
    return Benchmark::common_code_from_string(Benchmark::NS);
}

///////////////////////////////////////////////////////////////////////////////

double benchmark_common_code_to_raw_code_ns() {
    return Benchmark::common_code_to_raw_code(Benchmark::NS);
}
double benchmark_raw_code_to_common_code_ns() {
    return Benchmark::raw_code_to_common_code(Benchmark::NS);
}
double benchmark_common_code_to_short_code_us() {
    return Benchmark::common_code_to_short_code(Benchmark::US);
}
double benchmark_short_code_to_common_code_us() {
    return Benchmark::short_code_to_common_code(Benchmark::US);
}
double benchmark_common_code_to_short_code_fast_ns() {
    return Benchmark::common_code_to_short_code_fast(Benchmark::NS);
}
double benchmark_short_code_to_common_code_fast_ns() {
    return Benchmark::short_code_to_common_code_fast(Benchmark::NS);
}

///////////////////////////////////////////////////////////////////////////////

double benchmark_vertical_mirror_check_ns() {
    return Benchmark::vertical_mirror_check(Benchmark::NS);
}
double benchmark_horizontal_mirror_check_ns() {
    return Benchmark::horizontal_mirror_check(Benchmark::NS);
}
double benchmark_vertical_mirror_convert_ns() {
    return Benchmark::vertical_mirror_convert(Benchmark::NS);
}
double benchmark_horizontal_mirror_convert_ns() {
    return Benchmark::horizontal_mirror_convert(Benchmark::NS);
}

///////////////////////////////////////////////////////////////////////////////
