#include <iostream>

#include <benchmark/benchmark.h>

#define private public
#include "group/group.h"

#include <ranges/ranges.h>

#include "../../../third_party/thread-pool/include/BS_thread_pool.hpp"
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

            // volatile auto ret = klotski::cases::common_code_to_type_id(code);
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
            // volatile auto ret = klotski::cases::raw_code_to_type_id(code);
        }

    }

    state.SetItemsProcessed(state.iterations() * state.range(0));

}

static void GroupExtend(benchmark::State &state) {

    auto src = klotski::codec::RawCode::from_common_code(0x1A9BF0C00)->unwrap();

    for (auto _ : state) {

        // volatile auto ret = klotski::cases::group_extend_from_seed(src);

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

        klotski::cases::Ranges kk {};
        kk.reserve(7311921);

        for (auto [n, n_2x1, n_1x1] : nums) {
            kk.spawn(n, n_2x1, n_1x1);
        }
    }

}

static void OriginBasicRanges(benchmark::State &state) {

    BS::thread_pool pool {4};

    for (auto _ : state) {
        auto &kk = klotski::cases::BasicRanges::instance();
        kk.build_ranges(kk.get_ranges());

        // kk.build_async([](auto func) {func();}, [](){});

        // kk.build_async([&pool](auto func) {
        //     pool.submit_task(func);
        // }, [] {});
        // pool.wait();

    }
}

static void OriginAllCases(benchmark::State &state) {

    klotski::cases::BasicRanges::instance().build();
    // klotski::cases::get_reversed();

    // BS::thread_pool pool {4};

    for (auto _ : state) {
        auto &pp = klotski::cases::AllCases::instance();
        pp.available_ = false;
        pp.build();

        // pp.build_parallel_async([](auto func) {func();}, []() {
        //     // std::cout << "hello" << std::endl;
        // });

        // pp.build_parallel_async([&pool](auto func) {
        //     pool.submit_task(func);
        // }, [] {});
        //
        // pool.wait();

    }

}

static void RangesDerive(benchmark::State &state) {

    auto &basic_ranges = klotski::cases::BasicRanges::instance().fetch();

    klotski::cases::Ranges flip {basic_ranges};
    for (auto &x : flip) {
        x = klotski::range_reverse(x);
    }

    klotski::cases::BidiRanges bidi_ranges;
    for (auto x : basic_ranges) {
        bidi_ranges.emplace_back(klotski::cases::bidi_t {.r1 = x, .r2 = klotski::range_reverse(x)});
    }

    klotski::cases::Ranges results;
    // results.reserve(klotski::cases::ALL_CASES_NUM[5]);
    results.reserve(klotski::cases::ALL_CASES_NUM_);

    for (auto _ : state) {

        results.clear();
        // results.reserve(klotski::cases::ALL_CASES_NUM[5]);

        // basic_ranges.derive(5, results);

        klotski::cases::derive_demo(basic_ranges, flip, results, 0);
        klotski::cases::derive_demo(basic_ranges, flip, results, 1);
        klotski::cases::derive_demo(basic_ranges, flip, results, 2);

        klotski::cases::derive_demo(basic_ranges, flip, results, 4);
        klotski::cases::derive_demo(basic_ranges, flip, results, 5);
        klotski::cases::derive_demo(basic_ranges, flip, results, 6);

        klotski::cases::derive_demo(basic_ranges, flip, results, 8);
        klotski::cases::derive_demo(basic_ranges, flip, results, 9);
        klotski::cases::derive_demo(basic_ranges, flip, results, 10);

        klotski::cases::derive_demo(basic_ranges, flip, results, 12);
        klotski::cases::derive_demo(basic_ranges, flip, results, 13);
        klotski::cases::derive_demo(basic_ranges, flip, results, 14);

        // klotski::cases::derive_demo_pro(bidi_ranges, results, 0);
        // klotski::cases::derive_demo_pro(bidi_ranges, results, 1);
        // klotski::cases::derive_demo_pro(bidi_ranges, results, 2);
        //
        // klotski::cases::derive_demo_pro(bidi_ranges, results, 4);
        // klotski::cases::derive_demo_pro(bidi_ranges, results, 5);
        // klotski::cases::derive_demo_pro(bidi_ranges, results, 6);
        //
        // klotski::cases::derive_demo_pro(bidi_ranges, results, 8);
        // klotski::cases::derive_demo_pro(bidi_ranges, results, 9);
        // klotski::cases::derive_demo_pro(bidi_ranges, results, 10);
        //
        // klotski::cases::derive_demo_pro(bidi_ranges, results, 12);
        // klotski::cases::derive_demo_pro(bidi_ranges, results, 13);
        // klotski::cases::derive_demo_pro(bidi_ranges, results, 14);

    }

    // std::cout << results.size() << " vs " << klotski::cases::ALL_CASES_NUM[5] << std::endl;
}

// BENCHMARK(CommonCodeToTypeId)->Arg(8)->Arg(64)->Arg(256);
// BENCHMARK(RawCodeToTypeId)->Arg(8)->Arg(64)->Arg(256);

// BENCHMARK(GroupExtend)->Unit(benchmark::kMillisecond);

// BENCHMARK(FilterFromAllCases)->Unit(benchmark::kMillisecond);

// BENCHMARK(SpawnRanges)->Unit(benchmark::kMillisecond);

BENCHMARK(OriginBasicRanges)->Unit(benchmark::kMillisecond);

// BENCHMARK(OriginAllCases)->Unit(benchmark::kMillisecond);

// BENCHMARK(RangesDerive)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
