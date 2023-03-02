#include <string>
#include <vector>
#include "benchmark.h"
#include "all_cases.h"
#include "short_code.h"
#include "common_code.h"

#include <iostream>

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
    /// data preparation
    std::vector<ShortCode> data;
    data.reserve(klotski::SHORT_CODE_LIMIT);
    for (uint32_t short_code = 0; short_code < klotski::SHORT_CODE_LIMIT; ++short_code) {
        data.emplace_back(ShortCode::unsafe_create(short_code));
    }

    /// start benchmark
    auto start = clock();
    for (const auto &short_code : data) {
        short_code.to_string();
    }
    return time_format(start, format) / (double)data.size();
}

double Benchmark::short_code_from_string(TIME format) noexcept {
    /// data preparation
    std::vector<std::string> data;
    data.reserve(klotski::SHORT_CODE_LIMIT);
    for (uint32_t short_code = 0; short_code < klotski::SHORT_CODE_LIMIT; ++short_code) {
        data.emplace_back(ShortCode::unsafe_create(short_code).to_string());
    }

    /// start benchmark
    auto start = clock();
    for (auto &&short_code : data) {
        ShortCode{std::forward<std::string>(short_code)};
    }
    return time_format(start, format) / (double)data.size();
}

double Benchmark::common_code_to_string(TIME format) noexcept {
    /// data preparation
    std::vector<CommonCode> data;
    data.reserve(klotski::ALL_CASES_SIZE_SUM);
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : AllCases::fetch()[head]) {
            data.emplace_back(CommonCode::unsafe_create(head << 32 | range));
        }
    }

    /// start benchmark
    auto start = clock();
    for (const auto &common_code : data) {
        common_code.to_string();
    }
    return time_format(start, format) / (double)data.size();
}

double Benchmark::common_code_from_string(TIME format) noexcept {
    /// data preparation
    std::vector<std::string> data;
    data.reserve(klotski::ALL_CASES_SIZE_SUM);
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : AllCases::fetch()[head]) {
            data.emplace_back(
                CommonCode::unsafe_create(head << 32 | range).to_string()
            );
        }
    }

    /// start benchmark
    auto start = clock();
    for (auto &&common_code : data) {
        CommonCode{std::forward<std::string>(common_code)};
    }
    return time_format(start, format) / (double)data.size();
}

double Benchmark::common_code_to_string_shorten(TIME format) noexcept {
    /// data preparation
    std::vector<CommonCode> data;
    data.reserve(klotski::ALL_CASES_SIZE_SUM);
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : AllCases::fetch()[head]) {
            data.emplace_back(CommonCode::unsafe_create(head << 32 | range));
        }
    }

    /// start benchmark
    auto start = clock();
    for (const auto &common_code : data) {
        common_code.to_string(true);
    }
    return time_format(start, format) / (double)data.size();
}

double Benchmark::common_code_from_string_shorten(TIME format) noexcept {
    /// data preparation
    std::vector<std::string> data;
    data.reserve(klotski::ALL_CASES_SIZE_SUM);
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : AllCases::fetch()[head]) {
            data.emplace_back(
                CommonCode::unsafe_create(head << 32 | range).to_string(true)
            );
        }
    }

    /// start benchmark
    auto start = clock();
    for (auto &&common_code : data) {
        CommonCode{std::forward<std::string>(common_code)};
    }
    return time_format(start, format) / (double)data.size();
}
