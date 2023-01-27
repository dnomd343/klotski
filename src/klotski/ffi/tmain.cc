#include <cstdio>
#include <cstdint>

#include <iostream>

#include "klotski.h"
//#include "core.h"

#include "common.h"
#include "benchmark.h"

using namespace klotski;

void tmain() {
    printf("tmain start\n");

//    uint64_t common_code = 0x1A9BC0C00;
//    klotski::Common::range_reverse(common_code);

    std::cout << Benchmark::basic_ranges(Benchmark::MS) << std::endl;
    std::cout << Benchmark::all_cases(Benchmark::MS) << std::endl;

    printf("tmain exit\n");
}
