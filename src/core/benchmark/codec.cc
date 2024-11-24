#include <iostream>

#include <benchmark/benchmark.h>

// #define private public
#include "group/group.h"
#include "all_cases/all_cases.h"
#include "common_code/common_code.h"
// #undef private

using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

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
    std::cout << "do cal complete" << std::endl;
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
    //     // std::cout << "index = " << index << std::endl;

        // uint64_t kk[] {343, 666, 114514, 35324, 123454, 76453, 93411};
        // uint64_t index = kk[i % 7];

        result.emplace_back(codes[index]);
    }

    return result;

}

std::vector<uint32_t> short_code_samples(uint64_t num) {

    uint32_t part_size = klotski::codec::SHORT_CODE_LIMIT / num;

    // uint64_t offset = 0;
    uint32_t offset = part_size / 2;

    std::vector<uint32_t> result;

    for (uint32_t i = 0; i < num; ++i) {
        uint32_t index = i * part_size + offset;
        //     // std::cout << "index = " << index << std::endl;

        // uint64_t kk[] {343, 666, 114514, 35324, 123454, 76453, 93411};
        // uint64_t index = kk[i % 7];

        result.emplace_back(index);
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

std::vector<std::string> str_short_codes(uint64_t num) {

    auto src = short_code_samples(num);

    std::vector<std::string> codes;

    codes.reserve(src.size());
    for (auto x : src) {
        codes.emplace_back(klotski::codec::ShortCode::unsafe_create(x).to_string());
    }

    return codes;
}

static void CommonCodeSerialize(benchmark::State &state) {

    // common_code_samples(8);

    auto samples = common_code_samples(state.range(0));

    for (auto _ : state) {

        for (auto code : samples) {

            volatile auto ret = klotski::codec::CommonCode::string_encode(code);
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

    // auto samples = select_codes(state.range(0));
    auto samples = common_code_samples(state.range(0));

    for (auto _ : state) {
        for (auto code : samples) {
            volatile auto ret = klotski::codec::CommonCode::string_encode_shorten(code);
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

static void ShortCodeSerialize(benchmark::State &state) {
    std::vector<uint32_t> samples = short_code_samples(state.range(0));

    for (auto _ : state) {

        for (const auto code : samples) {
            volatile auto ret = ShortCode::string_encode(code);
        }

    }

    state.SetItemsProcessed(state.iterations() * state.range(0));

}

static void ShortCodeDeserialize(benchmark::State &state) {

    const auto tmp = str_short_codes(state.range(0));

    const std::vector<std::string_view> samples {tmp.begin(), tmp.end()};

    for (auto _ : state) {

        for (const auto code : samples) {

            volatile auto ret = ShortCode::string_decode(code);

        }

    }

    state.SetItemsProcessed(state.iterations() * state.range(0));

}

static void ShortCodeToCommonCode(benchmark::State &state) {

    // ShortCode::speed_up(true);
    ShortCode::speed_up(false);

    // ShortCode::fast_decode(4091296);

    auto short_code = CommonCode::unsafe_create(0x1A9BF0C00).to_short_code();

    for (auto _ : state) {

        // volatile auto kk = short_code.to_common_code();

        benchmark::DoNotOptimize(short_code.to_common_code());

        // if (AllCases::instance().is_available()) {
        // if (ShortCode::stage_ == ShortCode::Stage::FAST) {
        //     volatile auto pp = ShortCode::fast_decode(4091296);
        // }
        // }

    }

}

static void CommonCodeToShortCode(benchmark::State &state) {
    // ShortCode::speed_up(true);
    ShortCode::speed_up(false);

    auto common_code = CommonCode::unsafe_create(0x1A9BF0C00);

    // std::vector<CommonCode> samples;
    // for (auto code : common_code_samples(256)) {
    //     samples.emplace_back(CommonCode::unsafe_create(code));
    // }

    for (auto _ : state) {

        // for (auto common_code : samples) {
            // volatile auto kk = ShortCode(common_code);

            benchmark::DoNotOptimize(ShortCode(common_code));

        // }

    }
}

static void IsMirrorCompare(benchmark::State &state) {

    std::vector<CommonCode> samples;
    for (auto common_code : common_code_samples(512)) {
        samples.emplace_back(CommonCode::unsafe_create(common_code));
    }

    for (auto _ : state) {
        for (auto code : samples) {
            auto raw_code = code.to_raw_code();
            volatile auto ret = raw_code.is_horizontal_mirror();

            // volatile auto ret = CommonCode::is_mirror(code.unwrap());
        }
    }
}

static void CommonCodeCheck(benchmark::State &state) {

    std::vector<uint64_t> samples = common_code_samples(state.range(0));

    for (auto _ : state) {
        for (auto code : samples) {
            volatile auto tmp = CommonCode::check(code);
        }
    }

    state.SetItemsProcessed(state.iterations() * state.range(0));
}

BENCHMARK(CommonCodeCheck)->Range(64, 1024);

// BENCHMARK(CommonCodeSerialize)->Range(8, 256);
//BENCHMARK(CommonCodeDeserialize)->Range(8, 256);
// BENCHMARK(CommonCodeSerializeShorten)->Range(8, 256);
// BENCHMARK(CommonCodeDeserializeShorten)->Range(8, 256);

//BENCHMARK(ShortCodeSerialize)->Range(8, 256);
// BENCHMARK(ShortCodeDeserialize)->Range(8, 256);

// BENCHMARK(IsMirrorCompare);

// BENCHMARK(ShortCodeToCommonCode);
// BENCHMARK(CommonCodeToShortCode);

// static void CommonCodeDecode(benchmark::State &state) {
//     const auto tmp = str_common_codes(state.range(0));
//     const std::vector<std::string_view> samples {tmp.begin(), tmp.end()};
//
//     for (auto _ : state) {
//         for (const auto code : samples) {
//             benchmark::DoNotOptimize(CommonCode::string_decode(code));
//         }
//     }
//     state.SetBytesProcessed(int64_t(state.iterations()) *
//                           int64_t(state.range(0)));
//
//     state.SetComplexityN(state.range(0));
//
// }

// BENCHMARK(CommonCodeStrEncode)->Range(8, 256);
// BENCHMARK(CommonCodeSStrEncode)->Range(8, 256)
//   ->ComputeStatistics("ratio", [](const std::vector<double> &v) -> double {
//     return (*std::begin(v)) / (*std::end(v));
//   }, benchmark::StatisticUnit::kPercentage);

// BENCHMARK(CommonCodeSStrEncode)->Range(8, 256)->Unit(benchmark::kMillisecond);

// BENCHMARK(CommonCodeDeserialize)->RangeMultiplier(8)->Range(1, 256);
// BENCHMARK(CommonCodeDeserialize)->Name("Demo")->RangeMultiplier(8)->Range(1, 256);
// BENCHMARK(CommonCodeDeserialize)->RangeMultiplier(2)->Range(1, 256)->Complexity(benchmark::oN);

BENCHMARK_MAIN();
