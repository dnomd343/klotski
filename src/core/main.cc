#include <iostream>

#include "all_cases/all_cases.h"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

int main() {

    BasicRanges::Instance().Build();

    auto start = clock();

//    std::cout << BasicRanges::Instance().IsAvailable() << std::endl;
//    BasicRanges::Instance().Build();
//    std::cout << BasicRanges::Instance().IsAvailable() << std::endl;
//    for (auto x : BasicRanges::Instance().Fetch()) {
//        printf("%08X\n", x);
//    }

    AllCases::Instance().Build();
    for (uint64_t head = 0; head < 15; ++head) {
        for (auto range : AllCases::Instance().Fetch()[head]) {
            printf("%09llX\n", head << 32 | range);
        }
    }

    std::cerr << ((clock() - start) * 1000 / CLOCKS_PER_SEC) << "ms" << std::endl;

    return 0;
}
