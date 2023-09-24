#include <iostream>
#include <thread>
#include <future>

#include "all_cases/all_cases.h"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

void wkk(std::function<void()> &&f) {
    f();
}

int main() {

    std::vector<std::thread> threads;

    BasicRanges::Instance().Build();

    auto start = clock();

//    std::cout << BasicRanges::Instance().IsAvailable() << std::endl;
//    BasicRanges::Instance().Build();
//    std::cout << BasicRanges::Instance().IsAvailable() << std::endl;
//    for (auto x : BasicRanges::Instance().Fetch()) {
//        printf("%08X\n", x);
//    }

//    BasicRanges::Instance().Build();

    auto exec = [&threads](std::function<void()> &&f) {
//        std::cout << "start exec" << std::endl;

//        f();
        auto kk = std::thread(std::move(f));
//        threads.emplace_back(std::move(kk));
        kk.detach();

//        std::cout << "thread created" << std::endl;
    };

//    AllCases::Instance().Build();
    AllCases::Instance().BuildParallel(exec);

    std::cerr << ((clock() - start) * 1000 / CLOCKS_PER_SEC) << "ms" << std::endl;

//    for (auto &x : threads) {
//        x.join();
//    }

//    for (uint64_t head = 0; head < 15; ++head) {
//        for (auto range : AllCases::Instance().Fetch()[head]) {
//            printf("%09llX\n", head << 32 | range);
//        }
//    }

    return 0;
}
