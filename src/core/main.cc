#include <iostream>
#include "common_code.h"
#include "all_cases/all_cases.h"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::codec::CommonCode;

int main() {

//    std::cout << (int)'0' << std::endl;
//    std::cout << (int)'A' << std::endl;
//    std::cout << CommonCode::string_encode(0x1A9BF0C00, false) << std::endl;
//    std::cout << CommonCode::string_encode(0x0'10'00'00'00, false) << std::endl;
//    return 0;

//    printf("%09llX\n", CommonCode::string_decode("1A9BF0C").value());
//    return 0;

//    BasicRanges::Instance().Build();
    AllCases::Instance().Build();

    std::vector<uint64_t> common_codes;
    common_codes.reserve(klotski::cases::ALL_CASES_NUM_);

    for (uint64_t head = 0; head < 15; ++head) {
        for (auto range : AllCases::Instance().Fetch()[head]) {
            common_codes.emplace_back(head << 32 | range);
        }
    }

    std::vector<std::string> common_codes_str;
    common_codes_str.reserve(klotski::cases::ALL_CASES_NUM_);
    for (auto x : common_codes) {
        common_codes_str.emplace_back(CommonCode::string_encode(x, false));
    }

    auto start = clock();

//    for (auto common_code : common_codes) {
//        printf("%llX\n", common_code);
//        CommonCode::string_encode(common_code, true);
//        CommonCode::string_encode(common_code, false);
//        printf("%s\n", CommonCode::string_encode(common_code, false).c_str());
//        std::cout << CommonCode::string_encode(common_code, false) << std::endl;
//    }

    for (auto &common_code_str : common_codes_str) {
        CommonCode::string_decode(common_code_str);
    }

//    BasicRanges::Instance().Build();

//    AllCases::Instance().Build();
//    AllCases::Instance().BuildParallel([](auto f) {f();});
//    AllCases::Instance().BuildParallelAsync([](auto f) {f();}, []() {});

    std::cerr << ((clock() - start) * 1000 / CLOCKS_PER_SEC) << "ms" << std::endl;

//    for (uint64_t head = 0; head < 15; ++head) {
//        for (auto range : AllCases::Instance().Fetch()[head]) {
//            printf("%09llX\n", head << 32 | range);
//        }
//    }

    return 0;
}
