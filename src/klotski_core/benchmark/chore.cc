#include <chrono>
#include <random>
#include "benchmark.h"

namespace klotski {

bool Benchmark::data_ready_ = false;
std::mutex Benchmark::data_building_;
std::vector<RawCode> Benchmark::all_raw_codes_;
std::vector<ShortCode> Benchmark::all_short_codes_;
std::vector<CommonCode> Benchmark::all_common_codes_;
std::vector<std::string> Benchmark::all_short_codes_str_;
std::vector<std::string> Benchmark::all_common_codes_str_;

uint32_t Benchmark::random_seed() noexcept {
    using namespace std::chrono;
    auto tmp = system_clock::now().time_since_epoch();
    return duration_cast<microseconds>(tmp).count();
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

/// Generate a specified count of 32-bit random numbers.
std::vector<uint32_t> Benchmark::generate_u32_rand(uint32_t count) noexcept {
    std::vector<uint32_t> result;
    result.reserve(count);

    auto seed = random_seed();
    auto high_bit = (uint32_t)seed << 31;

    std::srand(seed);
    for (uint32_t i = 0; i < count; ++i) {
        auto tmp = high_bit | static_cast<uint32_t>(std::rand()); // int32_t -> uint32_t
        result.emplace_back(tmp);
        high_bit = tmp << 31;
    }
    return result;
}

/// Generate a specified count of 64-bit random numbers.
std::vector<uint64_t> Benchmark::generate_u64_rand(uint32_t count) noexcept {
    std::vector<uint64_t> result;
    result.reserve(count);
    auto tmp = generate_u32_rand(count * 2);
    for (uint32_t i = 0; i < count; ++i) {
        result.emplace_back((uint64_t)tmp[i * 2] << 32 | tmp[i * 2 + 1]); // combine two uint32_t
    }
    return result;
}

void Benchmark::data_initialize() noexcept {
    if (!data_ready_) {
        if (data_building_.try_lock()) { // mutex lock success
            build_data(); // start build process
            data_ready_ = true;
        } else {
            data_building_.lock(); // blocking waiting
        }
        data_building_.unlock(); // release mutex
    }
}

void Benchmark::build_data() noexcept {
    /// short code data preparation
    std::vector<uint32_t> tmp(SHORT_CODE_LIMIT);
    std::iota(tmp.begin(), tmp.end(), 0);

    all_short_codes_.reserve(SHORT_CODE_LIMIT);
    for (auto &&short_code : tmp) { // 0 ~ (SHORT_CODE_LIMIT - 1)
        all_short_codes_.emplace_back(ShortCode::unsafe_create(short_code));
    }

    all_short_codes_str_.reserve(SHORT_CODE_LIMIT);
    for (auto &&short_code : tmp) { // 0 ~ (SHORT_CODE_LIMIT - 1)
        all_short_codes_str_.emplace_back(
            ShortCode::unsafe_create(short_code).to_string()
        );
    }

    /// common code info preparation
    all_common_codes_.reserve(ALL_CASES_SIZE_SUM);
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : AllCases::fetch()[head]) {
            all_common_codes_.emplace_back(
                CommonCode::unsafe_create(head << 32 | range)
            );
        }
    }

    all_common_codes_str_.reserve(ALL_CASES_SIZE_SUM);
    for (auto &&common_code : all_common_codes_) {
        all_common_codes_str_.emplace_back(common_code.to_string());
    }

    /// raw code data preparation
    all_raw_codes_.reserve(ALL_CASES_SIZE_SUM);
    for (auto &&common_code : all_common_codes_) {
        all_raw_codes_.emplace_back(common_code.to_raw_code());
    }
}

} // namespace klotski
