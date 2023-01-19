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

    std::vector<RawCode> test_cases;
    {
        AllCases::build();
        std::vector<uint64_t> all_cases;
        for (uint64_t head = 0; head < 16; ++head) {
            for (const auto &range : AllCases::fetch()[head]) {
                all_cases.emplace_back(head << 32 | range);
            }
        }
        for (uint32_t i = 0; i < 10000; ++i) {
            test_cases.emplace_back(
                    RawCode::from_common_code(all_cases.at(i * 2933))
            );
        }
    }
    std::cout << "test size -> " << test_cases.size() << std::endl;


//    std::cout << "wait 3s" << std::endl;
//    sleep(3);

    std::cout << "start benchmark" << std::endl;
    auto start_time = clock();

//    AllCases::build();


    {
        auto fc = FastCal();
        for (uint32_t i = 0; i < test_cases.size(); ++i) {
            fc.solve(test_cases[i]);
            if (i % 1000 == 0) {
                std::cout << (i * 100 / test_cases.size()) << "%" << std::endl;
            }
        }
    }


//    auto raw_code = CommonCode("A5D3AF0").to_raw_code().unwrap();

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


//    std::cout << ShortCode("EP4HZ") << std::endl;
//    std::cout << ShortCode(14323231) << std::endl;

//    std::cout << RawCode::from_common_code("4fea134") << std::endl;
//    std::cout << CommonCode::from_raw_code(0xE58FC85FFEBC4DB) << std::endl;
//    std::cout << ShortCode::from_common_code("4fea134") << std::endl;
//    std::cout << CommonCode::from_short_code("AXCZN") << std::endl;


    std::cerr << (clock() - start_time) / CLOCKS_PER_SEC << "s" << std::endl;
//    std::cerr << (clock() - start_time) * 1000 / CLOCKS_PER_SEC << "ms" << std::endl;
//    std::cerr << (clock() - start_time) * 1000000 / CLOCKS_PER_SEC << "us" << std::endl;

    std::cout << "complete benchmark" << std::endl;

//    pause();

    return 0;
}
