#include <iostream>

#include <benchmark/benchmark.h>

// #define private public
#include "group/group.h"

#include <ranges/ranges.h>

#include "../../../third_party/thread-pool/include/BS_thread_pool.hpp"
#include "all_cases/all_cases.h"
// #undef private

using klotski::cases::AllCases;

using klotski::group::CaseInfo;

using klotski::group::Group;
using klotski::group::GroupCases;
using klotski::group::GroupUnion;

using klotski::codec::RawCode;
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

std::vector<klotski::codec::CommonCode> common_code_samples(uint64_t num) {

    static auto codes = all_common_codes();

    uint64_t part_size = codes.size() / num;

    // uint64_t offset = 0;
    uint64_t offset = part_size / 2;

    std::vector<klotski::codec::CommonCode> result;

    for (uint64_t i = 0; i < num; ++i) {
        uint64_t index = i * part_size + offset;
        //     // std::cout << "index = " << index << std::endl;

        // uint64_t kk[] {343, 666, 114514, 35324, 123454, 76453, 93411};
        // uint64_t index = kk[i % 7];

        result.emplace_back(klotski::codec::CommonCode::unsafe_create(codes[index]));
    }

    return result;
}

std::vector<klotski::codec::RawCode> raw_code_samples(uint64_t num) {

    auto codes = common_code_samples(num);

    std::vector<klotski::codec::RawCode> raw_codes;

    raw_codes.reserve(codes.size());
    for (auto code : codes) {
        raw_codes.emplace_back(code.to_raw_code());
    }

    return raw_codes;
}

static void CommonCodeToTypeId(benchmark::State &state) {

    auto samples = common_code_samples(state.range(0));

    for (auto _ : state) {

        for (auto code : samples) {
            volatile auto ret = GroupUnion::type_id(code);
        }

    }

    state.SetItemsProcessed(state.iterations() * state.range(0));

}

static void RawCodeToTypeId(benchmark::State &state) {

    auto samples = raw_code_samples(state.range(0));

    for (auto code : samples) {
        if (klotski::codec::RawCode::check(code.code_) == false) {
            std::cout << "error" << std::endl;
        }
    }

    for (auto _ : state) {

        for (auto code : samples) {
            volatile auto ret = GroupUnion::type_id(code);
        }

    }

    state.SetItemsProcessed(state.iterations() * state.range(0));

}

static void GroupExtend(benchmark::State &state) {

    auto src = klotski::codec::RawCode::from_common_code(0x1A9BF0C00).value();

    for (auto _ : state) {

        // volatile auto ret = klotski::group::Group_extend(src, 0);

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
        // kk.build_ranges(kk.get_ranges());

        kk.available_ = false;
        // kk.build();

        // kk.build_async([](auto func) {func();}, [](){});

        kk.build_async([&pool](auto func) {
            pool.submit_task(func);
        }, [] {});
        pool.wait();

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

    // klotski::cases::Ranges results;
    // results.reserve(klotski::cases::ALL_CASES_NUM_);

    auto group_union = klotski::group::GroupUnion::unsafe_create(169);

    std::vector<klotski::group::GroupUnion> unions;
    unions.reserve(klotski::group::TYPE_ID_LIMIT);
    for (int type_id = 0; type_id < klotski::group::TYPE_ID_LIMIT; ++type_id) {
        unions.emplace_back(klotski::group::GroupUnion::create(type_id).value());
    }

    for (auto _ : state) {

        // results.clear();
        // results.reserve(klotski::cases::ALL_CASES_NUM[5]);

        // volatile auto tmp = group_union.cases();

        for (auto g_union : unions) {
            volatile auto tmp = g_union.cases();
        }

    }

    // std::cout << results.size() << " vs " << klotski::cases::ALL_CASES_NUM[5] << std::endl;
}

static void SpawnGroups(benchmark::State &state) {

    volatile int type_id = 169;
    auto gu = GroupUnion::unsafe_create(type_id);

    for (auto _ : state) {
        // volatile auto kk = gu.groups();

        for (auto pattern_id = 0; pattern_id < gu.pattern_num(); ++pattern_id) {
            volatile auto kk = gu.groups(pattern_id);
        }

    }

}

static void GroupFromRawCode(benchmark::State &state) {

    auto code = CommonCode::unsafe_create(0x1A9BF0C00).to_raw_code();

    for (auto _ : state) {

        volatile auto g = Group::from_raw_code(code);

    }

}

static void IsVerticalMirror(benchmark::State &state) {
    volatile int type_id = 169;
    const auto groups = GroupUnion::unsafe_create(type_id).groups();

    for (auto _ : state) {
        for (auto group : groups) {
            volatile auto kk = group.is_vertical_mirror();
        }
    }
}

static void IsHorizontalMirror(benchmark::State &state) {
    volatile int type_id = 169;
    const auto groups = GroupUnion::unsafe_create(type_id).groups();

    for (auto _ : state) {
        for (auto group : groups) {
            volatile auto kk = group.is_horizontal_mirror();
        }
    }
}

static void ToVerticalMirror(benchmark::State &state) {
    volatile int type_id = 169;
    const auto groups = GroupUnion::unsafe_create(type_id).groups();

    for (auto _ : state) {
        for (auto group : groups) {
            volatile auto kk = group.to_vertical_mirror();
        }
    }
}

static void ToHorizontalMirror(benchmark::State &state) {
    volatile int type_id = 169;
    const auto groups = GroupUnion::unsafe_create(type_id).groups();

    for (auto _ : state) {
        for (auto group : groups) {
            volatile auto kk = group.to_horizontal_mirror();
        }
    }
}

static void FastObtainCode(benchmark::State &state) {
    // GroupCases::build();

    // std::vector<CaseInfo> infos;
    // for (auto code : common_code_samples(64)) {
    //     infos.emplace_back(GroupCases::tiny_obtain_info(code));
    // }
    // infos.emplace_back(CaseInfo::unsafe_create())

    const auto group = Group::unsafe_create(169, 0, Group::Toward::C);
    const klotski::cases::RangesUnion data = group.cases();

    std::array<size_t, 16> sizes {};
    size_t offset = 0;
    for (int i = 0; i < 16; ++i) {
        sizes[i] = offset;
        offset += data.ranges(i).size();
    }

    std::vector infos {
        CaseInfo::unsafe_create(group, 2631),
        CaseInfo::unsafe_create(group, 4203),
        CaseInfo::unsafe_create(group, 4504),
        CaseInfo::unsafe_create(group, 5178),
        CaseInfo::unsafe_create(group, 5411),
        CaseInfo::unsafe_create(group, 7208),
        CaseInfo::unsafe_create(group, 8385),
        CaseInfo::unsafe_create(group, 9821),
        CaseInfo::unsafe_create(group, 14220),
        CaseInfo::unsafe_create(group, 16159),
        CaseInfo::unsafe_create(group, 16224),
        CaseInfo::unsafe_create(group, 18027),
        CaseInfo::unsafe_create(group, 20271),
        CaseInfo::unsafe_create(group, 20980),
        CaseInfo::unsafe_create(group, 21130),
        CaseInfo::unsafe_create(group, 22794),
    };

    for (auto _ : state) {
        // for (auto info : infos) {
        //     volatile auto kk = GroupCases::fast_obtain_code(info);
        // }
        // volatile auto kk = GroupCases::fast_obtain_code(info);

        for (auto info : infos) {
            /// about 35ns
            auto &cases = data;
            uint64_t head = 0;
            auto case_id = info.case_id();
            for (;;) {
                if (case_id >= cases.ranges(head).size()) {
                    case_id -= cases.ranges(head).size();
                    ++head;
                } else {
                    break;
                }
            }
            auto range = cases.ranges(head)[case_id];
            volatile auto kk = CommonCode::unsafe_create(head << 32 | range);

            /// about 117ns
            // uint64_t head = std::upper_bound(sizes.begin(), sizes.end(), info.case_id()) - sizes.begin() - 1;
            // uint32_t range = data[head][info.case_id() - sizes[head]];
            // volatile auto kk = CommonCode::unsafe_create(head << 32 | range);
        }

    }
}

// BENCHMARK(CommonCodeToTypeId)->Arg(8)->Arg(64)->Arg(256);
// BENCHMARK(RawCodeToTypeId)->Arg(8)->Arg(64)->Arg(256);

// BENCHMARK(GroupExtend)->Unit(benchmark::kMillisecond);

// BENCHMARK(FilterFromAllCases)->Unit(benchmark::kMillisecond);

// BENCHMARK(SpawnRanges)->Unit(benchmark::kMillisecond);

// BENCHMARK(OriginBasicRanges)->Unit(benchmark::kMillisecond);

// BENCHMARK(OriginAllCases)->Unit(benchmark::kMillisecond);

// BENCHMARK(RangesDerive)->Unit(benchmark::kMillisecond);

// BENCHMARK(SpawnGroups);

// BENCHMARK(GroupFromRawCode)->Unit(benchmark::kMillisecond);

BENCHMARK(FastObtainCode);

// BENCHMARK(IsVerticalMirror);
// BENCHMARK(IsHorizontalMirror);
// BENCHMARK(ToVerticalMirror);
// BENCHMARK(ToHorizontalMirror);

BENCHMARK_MAIN();
