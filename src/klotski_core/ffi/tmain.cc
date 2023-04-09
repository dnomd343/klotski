#include <cstdio>
#include <iostream>
#include <algorithm>

#include "klotski.h"
#include "core.h"

#include "common.h"
#include "benchmark.h"

#include "all_cases.h"
#include "common_code.h"

#include "group.h"

using klotski::Group;
using klotski::AllCases;

using klotski::RawCode;
using klotski::CommonCode;

using klotski::Benchmark;

void tmain() {

    Group::block_num(123);

//    Group::block_num(CommonCode(0x1A9BF0C00));
//    Group::block_num(RawCode::from_common_code(0x1A9BF0C00));

//    std::cout << std::endl;

//    Group::block_num(CommonCode(0x4FEA13400));
//    Group::block_num(RawCode::from_common_code(0x4FEA13400));

//    for (auto &&common_code : AllCases::release()) {
//        auto s1 = Group::block_num(common_code);
//        auto s2 = Group::block_num(common_code.to_raw_code());
//        if (s1.n_1x1 != s2.n_1x1 || s1.n_1x2 != s2.n_1x2 || s1.n_2x1 != s2.n_2x1) {
//            std::cout << "fuck" << std::endl;
//        }
//    }

//    auto cases = std::vector<RawCode>();
//    auto common_codes = AllCases::release();
//    for (uint64_t i = 0; i < common_codes.size(); i += 293345 * 2) {
//        cases.emplace_back(common_codes[i].to_raw_code());
//    }
//
//    std::cout << "select " << cases.size() << " cases" << std::endl;

    auto start = clock();

//    for (auto &&raw_code : cases) {
//        std::cout << Group::demo(raw_code) << std::endl;
//    }

//    Group::demo(RawCode::from_common_code(0x1A9BF0C00));
//    Group::demo(RawCode::from_common_code(0x4FEA13400));

//    auto ret = Group::group_cases(RawCode::from_common_code(0x1A9BF0C00));
//    std::cout << ret.size() << std::endl;
//    std::cout << ret[0] << std::endl;
//    std::cout << ret[1] << std::endl;

//    auto tmp = RawCode::from_common_code(0x1A9BF0C00);
//    for (int i = 0; i < 100; ++i) {
//        Group::group_cases(tmp);
//    }

//    std::cout << ((clock() - start) * 1000 / CLOCKS_PER_SEC) << "ms" << std::endl;

    return;

//    printf("tmain start\n");

//    std::cout << "warm up: " << Benchmark::warm_up(1000000) << "us" << std::endl;

    std::cout << "range flip: " <<
        Benchmark::range_flip() << "ns" << std::endl;

    std::cout << "basic ranges: " <<
        Benchmark::basic_ranges() << "ms" << std::endl;
    std::cout << "all cases: " <<
        Benchmark::all_cases() << "ms" << std::endl;

    Benchmark::data_initialize();

    std::cout << "raw code check: " <<
        Benchmark::raw_code_check() << "ns" << std::endl;
    std::cout << "short code check: " <<
        Benchmark::short_code_check() << "ns" << std::endl;
    std::cout << "common code check: " <<
        Benchmark::common_code_check() << "ns" << std::endl;

    std::cout << "raw code check random: " <<
        Benchmark::raw_code_check_random() << "ns" << std::endl;
    std::cout << "short code check random: " <<
        Benchmark::short_code_check_random() << "ns" << std::endl;
    std::cout << "common code check random: " <<
        Benchmark::common_code_check_random() << "ns" << std::endl;

    std::cout << "short code to string: " <<
        Benchmark::short_code_to_string() << "ns" << std::endl;
    std::cout << "short code from string: " <<
        Benchmark::short_code_from_string() << "ns" << std::endl;

    std::cout << "common code to string: " <<
        Benchmark::common_code_to_string() << "ns" << std::endl;
    std::cout << "common code from string: " <<
        Benchmark::common_code_from_string() << "ns" << std::endl;

    std::cout << "common code to raw code: " <<
        Benchmark::common_code_to_raw_code() << "ns" << std::endl;
    std::cout << "raw code to common code: " <<
        Benchmark::raw_code_to_common_code() << "ns" << std::endl;

    std::cout << "common code to short code: " <<
        Benchmark::common_code_to_short_code() << "us" << std::endl;
    std::cout << "short code to common code: " <<
        Benchmark::short_code_to_common_code() << "us" << std::endl;

    std::cout << "common code to short code fast: " <<
        Benchmark::common_code_to_short_code_fast() << "ns" << std::endl;
    std::cout << "short code to common code fast: " <<
        Benchmark::short_code_to_common_code_fast() << "ns" << std::endl;

    std::cout << "vertical mirror check: " <<
        Benchmark::vertical_mirror_check() << "ns" << std::endl;
    std::cout << "horizontal mirror check: " <<
          Benchmark::horizontal_mirror_check() << "ns" << std::endl;
    std::cout << "vertical mirror convert: " <<
          Benchmark::vertical_mirror_convert() << "ns" << std::endl;
    std::cout << "horizontal mirror convert: " <<
          Benchmark::horizontal_mirror_convert() << "ns" << std::endl;

    return;

    std::vector<uint64_t> next;

    auto core = klotski::Core([&next](uint64_t code, uint64_t) {
        next.emplace_back(code);
    });

    auto next_num = [&core, &next](uint64_t raw_code) -> uint32_t {
        next.clear();
        core.next_cases(raw_code, 0);
        return next.size();
    };

    std::vector<uint64_t> all_cases;

    for (uint64_t head = 0; head < 16; ++head) {

        for (const auto &range : klotski::AllCases::fetch()[head]) {

            uint64_t raw_code = klotski::RawCode::from_common_code(head << 32 | range).unwrap();

            all_cases.emplace_back(raw_code);

        }

    }

    std::vector<uint32_t> next_size;

    std::cout << "start search" << std::endl;

//    auto start = clock();

    for (const auto &c : all_cases) {

        next_size.emplace_back(next_num(c));

    }

//    std::cout << ((clock() - start) * 1000 / CLOCKS_PER_SEC) << "ms" << std::endl;

//    std::sort(next_size.begin(), next_size.end());
//    std::cout << next_size[0] << std::endl;
//    std::cout << next_size[next_size.size() - 1] << std::endl;

    for (uint32_t i = 0; i < 69; ++i) {

        std::cout << i << ": " << std::count(next_size.begin(), next_size.end(), i) << std::endl;

    }

    printf("tmain exit\n");
}
