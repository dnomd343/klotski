#include <benchmark/benchmark.h>

#include "common_code/common_code.h"

#include "all_cases/all_cases.h"

using klotski::cases::AllCases;
using klotski::codec::CommonCode;

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

std::vector<std::string> str_common_codes(uint64_t num, bool shorten) {
    // auto src = select_codes(num);
    auto src = common_code_samples(num);

    std::vector<std::string> codes;

    codes.reserve(src.size());
    for (auto x : src) {
        codes.emplace_back(klotski::codec::CommonCode::unsafe_create(x).to_string(shorten));
    }

    return codes;
}

static void CommonCodeCheck(benchmark::State &state) {
    const auto samples = common_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(CommonCode::check(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void CommonCodeSerialize(benchmark::State &state) {
    const auto samples = common_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(CommonCode::string_encode(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void CommonCodeDeserialize(benchmark::State &state) {
    const auto tmp = str_common_codes(state.range(0), false);
    const std::vector<std::string_view> samples {tmp.begin(), tmp.end()};

    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(CommonCode::string_decode(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void CommonCodeSerializeShorten(benchmark::State &state) {
    const auto samples = common_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(CommonCode::string_encode_shorten(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void CommonCodeDeserializeShorten(benchmark::State &state) {
    const auto tmp = str_common_codes(state.range(0), true);
    const std::vector<std::string_view> samples {tmp.begin(), tmp.end()};

    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(CommonCode::string_decode(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void CommonCodeCheckMirror(benchmark::State &state) {
    const auto samples = common_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(CommonCode::check_mirror(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void CommonCodeVerticalMirror(benchmark::State &state) {
    const auto samples = common_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(CommonCode::get_vertical_mirror(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

static void CommonCodeHorizontalMirror(benchmark::State &state) {
    const auto samples = common_code_samples(state.range(0));
    for (auto _ : state) {
        for (const auto code : samples) {
            benchmark::DoNotOptimize(CommonCode::get_horizontal_mirror(code));
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

// BENCHMARK(CommonCodeCheck)->Range(8, 256);

// BENCHMARK(CommonCodeSerialize)->Range(8, 256);
// BENCHMARK(CommonCodeDeserialize)->Range(8, 256);
// BENCHMARK(CommonCodeSerializeShorten)->Range(8, 256);
// BENCHMARK(CommonCodeDeserializeShorten)->Range(8, 256);

// BENCHMARK(CommonCodeCheckMirror)->Range(64, 1024);

BENCHMARK(CommonCodeVerticalMirror)->Range(8, 256);
BENCHMARK(CommonCodeHorizontalMirror)->Range(8, 256);

BENCHMARK_MAIN();
