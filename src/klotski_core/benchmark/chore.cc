#include <thread>
#include "benchmark.h"

using klotski::RawCode;
using klotski::ShortCode;
using klotski::CommonCode;
using klotski::Benchmark;

bool Benchmark::data_ready = false;
std::vector<RawCode> Benchmark::all_raw_codes;
std::vector<ShortCode> Benchmark::all_short_codes;
std::vector<CommonCode> Benchmark::all_common_codes;
std::vector<std::string> Benchmark::all_short_codes_str;
std::vector<std::string> Benchmark::all_common_codes_str;

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

void Benchmark::data_preparation() noexcept {
    if (Benchmark::data_ready) {
        return;
    }

    /// short code data preparation
    std::vector<uint32_t> tmp(klotski::SHORT_CODE_LIMIT);
    std::iota(tmp.begin(), tmp.end(), 0);

    all_short_codes.reserve(klotski::SHORT_CODE_LIMIT);
    for (auto &&short_code : tmp) {
        all_short_codes.emplace_back(ShortCode::unsafe_create(short_code));
    }

    all_short_codes_str.reserve(klotski::SHORT_CODE_LIMIT);
    for (auto &&short_code : all_short_codes) {
        all_short_codes_str.emplace_back(short_code.to_string());
    }

    /// common code info preparation
    all_common_codes.reserve(klotski::ALL_CASES_SIZE_SUM);
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : AllCases::fetch()[head]) {
            all_common_codes.emplace_back(
                CommonCode::unsafe_create(head << 32 | range)
            );
        }
    }

    all_common_codes_str.reserve(klotski::ALL_CASES_SIZE_SUM);
    for (auto &&common_code : all_common_codes) {
        all_common_codes_str.emplace_back(common_code.to_string());
    }

    /// raw code data preparation
    all_raw_codes.reserve(klotski::ALL_CASES_SIZE_SUM);
    for (auto &&common_code : all_common_codes) {
        all_raw_codes.emplace_back(common_code.to_raw_code());
    }

    Benchmark::data_ready = true;
}
