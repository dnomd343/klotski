#include <cstdio>
#include <iostream>
#include <algorithm>

#include "klotski.h"
#include "core.h"

#include "common.h"
#include "benchmark.h"

#include "all_cases.h"
#include "common_code.h"

using klotski::Benchmark;

void tmain() {
//    printf("tmain start\n");

//    std::cout << "warm up: " << Benchmark::warm_up(1000000) << "us" << std::endl;

//    std::cout << "basic ranges: " << Benchmark::basic_ranges() << "ms" << std::endl;
//    std::cout << "all cases: " << Benchmark::all_cases() << "ms" << std::endl;

    std::cout << Benchmark::short_code_to_string() << "ns" << std::endl;
    std::cout << Benchmark::short_code_from_string() << "ns" << std::endl;

    std::cout << Benchmark::common_code_to_string() << "ns" << std::endl;
    std::cout << Benchmark::common_code_from_string() << "ns" << std::endl;

    std::cout << Benchmark::common_code_to_string_shorten() << "ns" << std::endl;
    std::cout << Benchmark::common_code_from_string_shorten() << "ns" << std::endl;

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

    auto start = clock();

    for (const auto &c : all_cases) {

        next_size.emplace_back(next_num(c));

    }

    std::cout << ((clock() - start) * 1000 / CLOCKS_PER_SEC) << "ms" << std::endl;

//    std::sort(next_size.begin(), next_size.end());
//    std::cout << next_size[0] << std::endl;
//    std::cout << next_size[next_size.size() - 1] << std::endl;

    for (uint32_t i = 0; i < 69; ++i) {

        std::cout << i << ": " << std::count(next_size.begin(), next_size.end(), i) << std::endl;

    }

    printf("tmain exit\n");
}
