#include <iostream>

#include "benchmark/benchmark.h"

#include "all_cases/all_cases.h"

#define private public
#include <group/group.h>

#include "common_code/common_code.h"

using klotski::cases::AllCases;

static std::vector<uint64_t> all_common_codes() {
    std::vector<uint64_t> all_codes;

    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto range : AllCases::instance().fetch()[head]) {
            all_codes.emplace_back(head << 32 | range);
        }
    }

    return all_codes;
}

static std::vector<uint64_t> select_codes() {
    auto codes = all_common_codes();

    return {codes[343], codes[666], codes[114514], codes[35324], codes[123454], codes[76453], codes[93411]};
}

std::vector<uint64_t> samples {
    0x00000303F,
    0x0000071F0,
    0x003339C40,
    0x000804340,
    0x0034C6D00,
    0x00230E0F0,
    0x002F371C0,
};

static void Demo(benchmark::State &state) {

    // auto samples = select_codes();
    //
    // for (auto code : samples) {
    //     auto c = klotski::codec::CommonCode::create(code).value();
    //     std::cout << c << std::endl;
    // }
    // std::cout << std::endl;

    // samples.clear();
    // samples = select_codes();
    // samples.emplace_back(0x00000303F);
    // samples.emplace_back(0x0000071F0);
    // samples.emplace_back(0x003339C40);
    // samples.emplace_back(0x000804340);
    // samples.emplace_back(0x0034C6D00);
    // samples.emplace_back(0x00230E0F0);
    // samples.emplace_back(0x002F371C0);

    for (auto _ : state) {

        for (auto code : samples) {
            // auto volatile holder = klotski::codec::CommonCode::string_encode(code);
            // auto volatile holder_1 = code + 213;
            // auto volatile holder_2 = code + 123;
            // auto volatile holder_3 = code + 233;
            // auto volatile holder_4 = code + 412;
            // auto volatile holder_5 = code + 896;
            // auto volatile holder_6 = code + 154;
            // auto volatile holder_7 = code + 124;

            auto ret = klotski::codec::CommonCode::string_encode(code);
            // auto volatile str_1 = ret[0];
            // auto volatile str_2 = ret[1];
            // auto volatile str_3 = ret[2];
        }

    }
}

BENCHMARK(Demo);

BENCHMARK_MAIN();
