#include "common.h"
#include "benchmark.h"
#include "all_cases.h"

using klotski::Benchmark;

/// ----------------------------- Benchmark Utils -----------------------------

double Benchmark::range_flip(TIME format) noexcept {
    uint32_t num = 0;
    auto start = clock();
    for (uint32_t i = 0; i < 0x1'0000'0000 - 0xF; i += 0xE) {
        Common::range_reverse(i);
        ++num;
    }
    return time_format(start, format) / (double)num;
}

/// --------------------------- Benchmark AllCases ----------------------------

double Benchmark::basic_ranges(TIME format) noexcept {
    if (BasicRanges::status() != BasicRanges::NOT_INIT) {
        return -1; // data already built -> skip
    }
    auto start = clock();
    BasicRanges::build();
    return time_format(start, format);
}

double Benchmark::all_cases(TIME format) noexcept {
    if (AllCases::status() != AllCases::NOT_INIT) {
        return -1; // data already built -> skip
    }
    BasicRanges::build(); // preparing benchmark data
    auto start = clock();
    AllCases::build();
    return time_format(start, format);
}

/// -------------------------- Benchmark Code Check ---------------------------

double Benchmark::raw_code_check(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    auto start = clock();
    for (auto &&raw_code : all_raw_codes) {
        raw_code.valid();
    }
    return time_format(start, format) / (double)all_raw_codes.size();
}

double Benchmark::short_code_check(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    auto start = clock();
    for (auto &&short_code : all_short_codes) {
        short_code.valid();
    }
    return time_format(start, format) / (double)all_short_codes.size();
}

double Benchmark::common_code_check(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    auto start = clock();
    for (auto &&common_code : all_common_codes) {
        common_code.valid();
    }
    return time_format(start, format) / (double)all_common_codes.size();
}

double Benchmark::raw_code_check_random(TIME format) noexcept {
    auto random_data = generate_u64_rand(klotski::ALL_CASES_SIZE_SUM);
    for (auto &&val : random_data) {
        val &= ~((uint64_t)0b1111 << 60); // clear high 4-bits
    }
    auto start = clock();
    for (auto &&raw_code : random_data) {
        RawCode::check(raw_code);
    }
    return time_format(start, format) / (double)random_data.size();
}

double Benchmark::short_code_check_random(TIME format) noexcept {
    auto random_data = generate_u32_rand(klotski::ALL_CASES_SIZE_SUM);
    auto start = clock();
    for (auto &&short_code : random_data) {
        ShortCode::check(short_code);
    }
    return time_format(start, format) / (double)random_data.size();
}

double Benchmark::common_code_check_random(TIME format) noexcept {
    auto random_data = generate_u64_rand(klotski::ALL_CASES_SIZE_SUM);
    for (auto &&val : random_data) {
        val &= ~((uint64_t)0xFFFFFFF << 36); // clear high 28-bits
    }
    auto start = clock();
    for (auto &&common_code : random_data) {
        CommonCode::check(common_code);
    }
    return time_format(start, format) / (double)random_data.size();
}

/// -------------------------- Benchmark Code String --------------------------

double Benchmark::short_code_to_string(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    auto start = clock();
    for (auto &&short_code : all_short_codes) {
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
    for (auto &&common_code : all_common_codes) {
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

/// ------------------------- Benchmark Code Convert --------------------------

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

/// ---------------------------- Benchmark Mirror -----------------------------

double Benchmark::vertical_mirror_check(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    auto start = clock();
    for (auto &&raw_code : all_raw_codes) {
        raw_code.is_vertical_mirror();
    }
    return time_format(start, format) / (double)all_raw_codes.size();
}

double Benchmark::horizontal_mirror_check(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    auto start = clock();
    for (auto &&raw_code : all_raw_codes) {
        raw_code.is_horizontal_mirror();
    }
    return time_format(start, format) / (double)all_raw_codes.size();
}

double Benchmark::vertical_mirror_convert(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    auto start = clock();
    for (auto &&raw_code : all_raw_codes) {
        raw_code.to_vertical_mirror();
    }
    return time_format(start, format) / (double)all_raw_codes.size();
}

double Benchmark::horizontal_mirror_convert(TIME format) noexcept {
    if (!data_ready) {
        return -1; // data no ready -> skip
    }
    auto start = clock();
    for (auto &&raw_code : all_raw_codes) {
        raw_code.to_horizontal_mirror();
    }
    return time_format(start, format) / (double)all_raw_codes.size();
}
