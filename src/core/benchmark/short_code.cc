#include <benchmark/benchmark.h>

#include "all_cases/all_cases.h"

#include "short_code/short_code.h"

using klotski::cases::AllCases;
using klotski::codec::ShortCode;

/// Build all valid CommonCodes.
static std::vector<uint64_t> all_common_codes() {
    std::vector<uint64_t> codes;
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto range : AllCases::instance().fetch().ranges(head)) {
            codes.emplace_back(head << 32 | range);
        }
    }
    // std::cout << "do cal complete" << std::endl;
    return codes;
}

std::vector<uint64_t> common_code_samples(uint64_t num) {

    static auto codes = all_common_codes();

    uint64_t part_size = codes.size() / num;

    uint64_t offset = part_size / 2;

    std::vector<uint64_t> result;

    for (uint64_t i = 0; i < num; ++i) {
        uint64_t index = i * part_size + offset;
        result.emplace_back(codes[index]);
    }

    return result;

}

std::vector<uint32_t> short_code_samples(uint64_t num) {

    uint32_t part_size = klotski::codec::SHORT_CODE_LIMIT / num;

    uint32_t offset = part_size / 2;

    std::vector<uint32_t> result;

    for (uint32_t i = 0; i < num; ++i) {
        uint32_t index = i * part_size + offset;

        result.emplace_back(index);
    }

    return result;

}

std::vector<std::string> str_short_codes(uint64_t num) {
    auto src = short_code_samples(num);

    std::vector<std::string> codes;
    codes.reserve(src.size());
    for (auto x : src) {
        codes.emplace_back(klotski::codec::ShortCode::unsafe_create(x).to_string());
    }

    return codes;
}

static void ShortCodeCheck(benchmark::State &state) {
    const auto samples = short_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(ShortCode::check(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void ShortCodeSerialize(benchmark::State &state) {
    const auto samples = short_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            // volatile auto ret = ShortCode::string_encode(code);
            benchmark::DoNotOptimize(ShortCode::string_encode(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void ShortCodeDeserialize(benchmark::State &state) {
    const auto tmp = str_short_codes(state.range(0));
    const std::vector<std::string_view> samples {tmp.begin(), tmp.end()};

    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(ShortCode::string_decode(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void ShortCodeFastDecode(benchmark::State &state) {
    ShortCode::speed_up(true);
    const auto samples = short_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(ShortCode::fast_decode(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void ShortCodeFastEncode(benchmark::State &state) {
    ShortCode::speed_up(true);
    const auto samples = common_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(ShortCode::fast_encode(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void ShortCodeTinyDecode(benchmark::State &state) {
    ShortCode::fast_ = false;
    ShortCode::speed_up(false);
    const auto samples = short_code_samples(state.range(0));
    for (auto _ : state) {
        for (auto code : samples) {
            benchmark::DoNotOptimize(ShortCode::tiny_decode(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void ShortCodeTinyEncode(benchmark::State &state) {
    ShortCode::fast_ = false;
    ShortCode::speed_up(false);
    const auto samples = common_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(ShortCode::tiny_encode(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

// BENCHMARK(ShortCodeCheck)->Range(8, 256);

// BENCHMARK(ShortCodeSerialize)->Range(8, 256);
// BENCHMARK(ShortCodeDeserialize)->Range(8, 256);

BENCHMARK(ShortCodeFastDecode)->Range(8, 256);
BENCHMARK(ShortCodeFastEncode)->Range(8, 256);

// BENCHMARK(ShortCodeTinyDecode)->Range(8, 256)->Unit(benchmark::kMicrosecond);
// BENCHMARK(ShortCodeTinyEncode)->Range(8, 256)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
