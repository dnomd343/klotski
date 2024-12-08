#include <benchmark/benchmark.h>

#include "raw_code/raw_code.h"

#include "all_cases/all_cases.h"

using klotski::codec::RawCode;
using klotski::cases::AllCases;

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

    // uint64_t offset = 0;
    uint64_t offset = part_size / 2;

    std::vector<uint64_t> result;

    for (uint64_t i = 0; i < num; ++i) {
        uint64_t index = i * part_size + offset;
        result.emplace_back(codes[index]);
    }

    return result;
}

std::vector<uint64_t> raw_code_samples(size_t num) {
    std::vector<uint64_t> raw_codes;
    raw_codes.reserve(num);

    for (auto common_code : common_code_samples(num)) {
        raw_codes.emplace_back(klotski::codec::CommonCode::unsafe_create(common_code).to_raw_code());
    }

    return raw_codes;
}

static void RawCodeCheck(benchmark::State &state) {
    const auto samples = raw_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(RawCode::check(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void RawCodeCompact(benchmark::State &state) {
    const auto samples = raw_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(RawCode::compact(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void RawCodeExtract(benchmark::State &state) {
    const auto samples = common_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(RawCode::extract(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void RawCodeCheckMirror(benchmark::State &state) {
    const auto samples = raw_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(RawCode::check_mirror(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void RawCodeVerticalMirror(benchmark::State &state) {
    const auto samples = raw_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(RawCode::get_vertical_mirror(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void RawCodeHorizontalMirror(benchmark::State &state) {
    const auto samples = raw_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(RawCode::get_horizontal_mirror(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

// BENCHMARK(RawCodeCheck)->Range(8, 256);

// BENCHMARK(RawCodeCompact)->RangeMultiplier(4)->Range(16, 1024);
// BENCHMARK(RawCodeExtract)->RangeMultiplier(4)->Range(16, 1024);

// BENCHMARK(RawCodeCheckMirror)->RangeMultiplier(4)->Range(16, 1024);

// BENCHMARK(RawCodeVerticalMirror)->RangeMultiplier(4)->Range(16, 1024);
BENCHMARK(RawCodeHorizontalMirror)->RangeMultiplier(4)->Range(16, 1024);

BENCHMARK_MAIN();
