#include "benchmark.h"
#include "all_cases.h"

using klotski::Benchmark;

double Benchmark::basic_ranges(TIME format) noexcept {
    if (BasicRanges::status() != BasicRanges::NO_INIT) {
        return -1; // data already built -> skip
    }
    auto start = clock();
    BasicRanges::build();
    return time_format(start, format);
}

double Benchmark::all_cases(TIME format) noexcept {
    if (AllCases::status() != AllCases::NO_INIT) {
        return -1; // data already built -> skip
    }
    BasicRanges::build(); // preparing benchmark data
    auto start = clock();
    AllCases::build();
    return time_format(start, format);
}

double Benchmark::short_code_to_string(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    auto start = clock();
    for (const auto &short_code : all_short_codes) {
        short_code.to_string();
    }
    return time_format(start, format) / (double)all_short_codes.size();
}

double Benchmark::short_code_from_string(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    auto start = clock();
    for (auto &&short_code : all_short_codes_str) {
        ShortCode{std::forward<std::string>(short_code)};
    }
    return time_format(start, format) / (double)all_short_codes_str.size();
}

double Benchmark::common_code_to_string(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    auto start = clock();
    for (const auto &common_code : all_common_codes) {
        common_code.to_string();
    }
    return time_format(start, format) / (double)all_common_codes.size();
}

double Benchmark::common_code_from_string(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    auto start = clock();
    for (auto &&common_code : all_common_codes_str) {
        CommonCode{std::forward<std::string>(common_code)};
    }
    return time_format(start, format) / (double)all_common_codes_str.size();
}
