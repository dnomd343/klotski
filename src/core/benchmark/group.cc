#include <iostream>

#include <benchmark/benchmark.h>
#include <group/group.h>

#include "all_cases/all_cases.h"

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

BENCHMARK(CommonCodeToTypeId)->Arg(8)->Arg(64)->Arg(256);
BENCHMARK(RawCodeToTypeId)->Arg(8)->Arg(64)->Arg(256);

BENCHMARK_MAIN();
