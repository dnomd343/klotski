#include <iostream>

#include "all_cases/all_cases.h"

using klotski::cases::BasicRanges;

int main() {
    auto start = clock();

//    std::cout << BasicRanges::Instance().IsAvailable() << std::endl;
//    BasicRanges::Instance().Build();
//    std::cout << BasicRanges::Instance().IsAvailable() << std::endl;
    for (auto x : BasicRanges::Instance().Fetch()) {
        printf("%08X\n", x);
    }

    std::cerr << ((clock() - start) * 1000 / CLOCKS_PER_SEC) << "ms" << std::endl;

    return 0;
}
