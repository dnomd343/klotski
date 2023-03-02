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

double Benchmark::common_code_to_raw_code(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    auto start = clock();
    for (auto &&common_code : all_common_codes) {
        RawCode{std::forward<CommonCode>(common_code)};
    }
    return time_format(start, format) / (double)all_common_codes.size();
}

double Benchmark::raw_code_to_common_code(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    auto start = clock();
    for (auto &&raw_code : all_raw_codes) {
        raw_code.to_common_code();
    }
    return time_format(start, format) / (double)all_raw_codes.size();
}

double Benchmark::common_code_to_short_code(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    uint32_t num = 0;
    ShortCode::speed_up(ShortCode::NORMAL);
    auto start = clock();
    for (uint64_t i = 0; i < all_common_codes.size(); i += 1000) {
        ShortCode{all_common_codes[i]};
        ++num;
    }
    return time_format(start, format) / (double)num;
}

double Benchmark::short_code_to_common_code(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    uint32_t num = 0;
    ShortCode::speed_up(ShortCode::NORMAL);
    auto start = clock();
    for (uint64_t i = 0; i < all_short_codes.size(); i += 1000) {
        all_short_codes[i].to_common_code();
        ++num;
    }
    return time_format(start, format) / (double)num;
}

double Benchmark::common_code_to_short_code_fast(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    ShortCode::speed_up(ShortCode::FAST);
    auto start = clock();
    for (auto &&common_code : all_common_codes) {
        ShortCode{std::forward<CommonCode>(common_code)};
    }
    return time_format(start, format) / (double)all_common_codes.size();
}

double Benchmark::short_code_to_common_code_fast(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    ShortCode::speed_up(ShortCode::FAST);
    auto start = clock();
    for (auto &&short_code : all_short_codes) {
        short_code.to_common_code();
    }
    return time_format(start, format) / (double)all_short_codes.size();
}
