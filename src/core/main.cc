#include <iostream>
#include "all_cases/all_cases.h"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

int main() {

    BasicRanges::Instance().Build();

    auto start = clock();

    AllCases::Instance().Build();
//    AllCases::Instance().BuildParallel([](auto f) {f();});

    std::cerr << ((clock() - start) * 1000 / CLOCKS_PER_SEC) << "ms" << std::endl;

//    for (uint64_t head = 0; head < 15; ++head) {
//        for (auto range : AllCases::Instance().Fetch()[head]) {
//            printf("%09llX\n", head << 32 | range);
//        }
//    }

    return 0;
}
