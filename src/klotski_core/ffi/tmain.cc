#include <cstdio>
#include <iostream>
#include <algorithm>

#include "klotski.h"
#include "core.h"

//#include "common.h"
#include "benchmark.h"

#include "all_cases.h"
#include "common_code.h"

void tmain() {
    printf("tmain start\n");

//    printf("%d\n", ALL_CASES_SIZE_SUM);
//    std::cout << ALL_CASES_SIZE_SUM << std::endl;

//    uint64_t common_code = 0x1A9BC0C00;
//    klotski::Common::range_reverse(common_code);

//    printf("%f\n", Benchmark::basic_ranges(Benchmark::MS));
//    printf("%f\n", Benchmark::all_cases(Benchmark::MS));
//    std::cout << Benchmark::basic_ranges(Benchmark::MS) << std::endl;
//    std::cout << Benchmark::all_cases(Benchmark::MS) << std::endl;


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
