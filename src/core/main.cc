#include <thread>
#include <iostream>
#include <algorithm>

#include "raw_code.h"
#include "all_cases.h"
#include "short_code.h"
#include "common_code.h"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::codec::RawCode;
using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

using klotski::codec::SHORT_CODE_LIMIT;

int main() {

    AllCases::instance().build();
//    BasicRanges::instance().build();

//    std::vector<uint64_t> common_codes;
//    common_codes.reserve(klotski::cases::ALL_CASES_NUM_);

//    for (uint64_t head = 0; head < 15; ++head) {
//        for (auto range : AllCases::instance().fetch()[head]) {
//            common_codes.emplace_back(head << 32 | range);
//        }
//    }

//    std::vector<std::string> common_codes_str;
//    common_codes_str.reserve(klotski::cases::ALL_CASES_NUM_);
//    for (auto x : common_codes) {
//        common_codes_str.emplace_back(CommonCode::string_encode(x, false));
//    }

    ShortCode::speed_up(true);

    auto start = clock();

    for (uint32_t short_code = 0; short_code < SHORT_CODE_LIMIT; ++short_code) {
        ShortCode::unsafe_create(short_code).to_common_code();
    }

//    for (auto common_code : common_codes) {
//        printf("%llX\n", common_code);
//        CommonCode::string_encode(common_code, true);
//        CommonCode::string_encode(common_code, false);
//        printf("%s\n", CommonCode::string_encode(common_code, false).c_str());
//        std::cout << CommonCode::string_encode(common_code, false) << std::endl;
//    }

//    for (auto &common_code_str : common_codes_str) {
//        CommonCode::string_decode(common_code_str);
//    }

//    BasicRanges::instance().build();
//    AllCases::instance().build();
//    AllCases::instance().build_parallel([](auto f) {f();});
//    AllCases::instance().build_parallel_async([](auto f) {f();}, []() {});

    std::cerr << ((clock() - start) * 1000 / CLOCKS_PER_SEC) << "ms" << std::endl;

    return 0;
}
