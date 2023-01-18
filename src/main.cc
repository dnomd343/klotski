#include <iostream>
#include "core.h"
#include "all_cases.h"
#include "basic_ranges.h"
#include "common_code.h"
#include "short_code.h"
#include "raw_code.h"
#include "fast_cal.h"
#include "analyse.h"
#include "common.h"

#include <thread>
#include <algorithm>
#include <unistd.h>

int main() {

//    BasicRanges::build();

//    std::cout << "wait 3s" << std::endl;
//    sleep(3);

//    std::cout << "start benchmark" << std::endl;
    auto start_time = clock();

//    AllCases::build();

    auto f = FastCal();
//    auto ret = f.solve((uint64_t)RawCode::from_common_code("1a9bf0c"));
    auto ret = f.furthest((uint64_t)RawCode::from_common_code("1a9bf0c"));

//    auto f = FastCal(RawCode::from_common_code("1a9bf0c").unwrap());
//    auto ret = f.solve();
//    std::cout << RawCode(ret) << std::endl;
    for (const auto &r : ret) {
        std::cout << RawCode(r) << std::endl;
    }

//    for (const auto &c : ret) {
//        std::cout << RawCode(c) << std::endl;
//    }

//    auto raw_code = CommonCode("1a9bf0c").to_raw_code().unwrap();
//    auto raw_code = CommonCode("A5D3AF0").to_raw_code().unwrap();
//    std::cout << fast_cal(raw_code) << std::endl;
//    auto f = FastCal();
//    f.fast_cal(raw_code);

//    auto a = Analyse();
//    a.start_analyse(raw_code);

//    a.backtrack(0x07F87E0E5BFFF492);

//    std::vector<uint64_t> raw_codes = {
//        CommonCode(0xDAAF4CC00).to_raw_code().unwrap(),
//        CommonCode(0xDAA7F3000).to_raw_code().unwrap(),
//    };
//    a.backtrack(raw_codes);

//    std::vector<uint64_t> raw_codes = {
//        CommonCode("DE77D80").to_raw_code().unwrap(),
//        CommonCode("DF6DD80").to_raw_code().unwrap(),
//        CommonCode("DF69730").to_raw_code().unwrap(),
//        CommonCode("DE76D30").to_raw_code().unwrap(),
//        CommonCode("D5AF730").to_raw_code().unwrap(),
//        CommonCode("D5B43BC").to_raw_code().unwrap(),
//    };
//    std::vector<uint64_t> raw_codes = {
//        CommonCode("EF697C0").to_raw_code().unwrap(),
//    };
    // TODO: FATAL ERROR -> don't using mask in Analyse
//    a.backtrack(raw_codes);


//    int sum = 0;
//    for (auto const &raw_code : all_cases_raw) {
//        std::cout << RawCode(raw_code).dump_case();
//        fast_cal(raw_code);
//        std::cout << fast_cal(raw_code) << std::endl;
//        std::cout << std::endl;
//        if (sum % 100000 == 0) {
//            std::cout << "sum = " << sum << std::endl;
//        }
//        ++sum;

//    }


//    std::cout << ShortCode("EP4HZ") << std::endl;
//    std::cout << ShortCode(14323231) << std::endl;

//    std::cout << RawCode::from_common_code("4fea134") << std::endl;
//    std::cout << CommonCode::from_raw_code(0xE58FC85FFEBC4DB) << std::endl;
//    std::cout << ShortCode::from_common_code("4fea134") << std::endl;
//    std::cout << CommonCode::from_short_code("AXCZN") << std::endl;


//    std::cerr << (clock() - start_time) / CLOCKS_PER_SEC << "s" << std::endl;
//    std::cerr << (clock() - start_time) * 1000 / CLOCKS_PER_SEC << "ms" << std::endl;
    std::cerr << (clock() - start_time) * 1000000 / CLOCKS_PER_SEC << "us" << std::endl;

//    std::cout << "complete benchmark" << std::endl;

//    pause();

    return 0;
}
