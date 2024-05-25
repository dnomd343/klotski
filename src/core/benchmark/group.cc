#include <iostream>

#include <benchmark/benchmark.h>

#define private public
#include "group/group.h"
#include "all_cases/all_cases.h"
#undef private

using klotski::cases::AllCases;

/// Build all valid CommonCodes.
static std::vector<uint64_t> all_common_codes() {
    std::vector<uint64_t> codes;
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto range : AllCases::instance().fetch()[head]) {
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

std::vector<uint64_t> raw_code_samples(uint64_t num) {

    auto codes = common_code_samples(num);

    for (auto &code : codes) {
        code = klotski::codec::CommonCode::unsafe_create(code).to_raw_code().unwrap();
    }

    return codes;
}

static void CommonCodeToTypeId(benchmark::State &state) {

    auto samples = common_code_samples(state.range(0));

    for (auto _ : state) {

        for (auto code : samples) {

            volatile auto ret = klotski::cases::common_code_to_type_id(code);
        }

    }

    state.SetItemsProcessed(state.iterations() * state.range(0));

}

static void RawCodeToTypeId(benchmark::State &state) {

    auto samples = raw_code_samples(state.range(0));

    // for (auto code : samples) {
    //     if (klotski::codec::RawCode::check(code) == false) {
    //         std::cout << "error" << std::endl;
    //     }
    // }

    for (auto _ : state) {

        for (auto code : samples) {
            volatile auto ret = klotski::cases::raw_code_to_type_id(code);
        }

    }

    state.SetItemsProcessed(state.iterations() * state.range(0));

}

static void GroupExtend(benchmark::State &state) {

    auto src = klotski::codec::RawCode::from_common_code(0x1A9BF0C00)->unwrap();

    for (auto _ : state) {

        volatile auto ret = klotski::cases::group_extend_from_seed(src);

        // std::cout << ret.size() << std::endl;
    }

    // state.SetItemsProcessed(state.iterations());

}

// static void FilterFromAllCases(benchmark::State &state) {
//
//     klotski::cases::AllCases::instance().build();
//
//     for (auto _ : state) {
//         for (uint64_t head = 0; head < 16; ++head) {
//
//             for (const auto range : AllCases::instance().fetch()[head]) {
//                 uint64_t common_code = head << 32 | range;
//
//                 volatile auto ret = klotski::cases::common_code_to_type_id(common_code);
//
//             }
//
//         }
//     }
//
// }

static std::vector<std::tuple<int, int, int>> target_nums() {

    std::vector<std::tuple<int, int, int>> results;

    for (int n = 0; n <= 7; ++n) {
        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) {
            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) {
                results.emplace_back(n, n_2x1, n_1x1);
            }
        }
    }

    results.resize(203);
    return results;
}

static void SpawnRanges(benchmark::State &state) {

    auto nums = target_nums();

    for (auto _ : state) {
        for (auto [n, n_2x1, n_1x1] : nums) {
            // klotski::cases::spawn_ranges(n, n_2x1, n_1x1);
        }
    }

}

static void BasicRanges(benchmark::State &state) {

    for (auto _ : state) {
        klotski::cases::basic_ranges();
    }

}

static void OriginBasicRanges(benchmark::State &state) {
    for (auto _ : state) {
        auto &kk = klotski::cases::BasicRanges::instance();
        kk.build_ranges(kk.get_ranges());
    }
}

// BENCHMARK(CommonCodeToTypeId)->Arg(8)->Arg(64)->Arg(256);
// BENCHMARK(RawCodeToTypeId)->Arg(8)->Arg(64)->Arg(256);

// BENCHMARK(GroupExtend)->Unit(benchmark::kMillisecond);

// BENCHMARK(FilterFromAllCases)->Unit(benchmark::kMillisecond);

// BENCHMARK(SpawnRanges)->Unit(benchmark::kMillisecond);

BENCHMARK(BasicRanges)->Unit(benchmark::kMillisecond);

// BENCHMARK(OriginBasicRanges)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
