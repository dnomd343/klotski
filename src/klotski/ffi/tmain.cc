#include <cstdio>

//#include <iostream>

#include "klotski.h"
//#include "core.h"

//#include "common.h"
#include "benchmark.h"

#include "all_cases.h"

using namespace klotski;

void tmain() {
    printf("tmain start\n");

    printf("%d\n", ALL_CASES_SIZE_SUM);
//    std::cout << ALL_CASES_SIZE_SUM << std::endl;

//    uint64_t common_code = 0x1A9BC0C00;
//    klotski::Common::range_reverse(common_code);

    printf("%f\n", Benchmark::basic_ranges(Benchmark::MS));
    printf("%f\n", Benchmark::all_cases(Benchmark::MS));
//    std::cout << Benchmark::basic_ranges(Benchmark::MS) << std::endl;
//    std::cout << Benchmark::all_cases(Benchmark::MS) << std::endl;

    printf("tmain exit\n");
}
