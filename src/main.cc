#include <iostream>

#include "common.h"
#include "all_cases.h"

#include "raw_code.h"
#include "short_code.h"
#include "common_code.h"

#include "core.h"
#include "analyse.h"
#include "fast_cal.h"

//#include "graph.h"

//#include "benchmark.h"

//#include <unistd.h>

int main() {

    std::cout << "start test" << std::endl;

    std::cout << (uint32_t)Common::check_range(1, Common::range_reverse(0xA9BF0C00)) << std::endl;

    AllCases::build();
    for (const auto &a : AllCases::fetch()) {
        std::cout << a.size() << std::endl;
    }

    std::cout << RawCode::from_common_code("1a9bf0c") << std::endl;
    std::cout << RawCode::create(0x0603EDF5CAFFF5E2) << std::endl;

    std::cout << CommonCode::create(0x1A9BF0C00) << std::endl;
    std::cout << CommonCode::from_string("1a9bf0c") << std::endl;
    std::cout << CommonCode::from_short_code(4091296) << std::endl;
    std::cout << CommonCode::from_raw_code(0x0603EDF5CAFFF5E2) << std::endl;

    std::cout << ShortCode::create(4091296) << std::endl;
    std::cout << ShortCode::from_string("4WVE1") << std::endl;
    std::cout << ShortCode::from_common_code(0x1A9BF0C00) << std::endl;

    auto core = Core([](uint64_t code, uint64_t mask) {
        printf("new case -> code = %015lX | mask = %015lX\n", code, mask);
    });
    core.next_cases(RawCode::from_common_code("1a9bf0c").unwrap(), 0);

    for (const auto &c : FastCal::resolve(RawCode::from_common_code("1a9bf0c"))) {
        std::cout << c << std::endl;
    }

    auto a = Analyse(RawCode::from_common_code("1a9bf0c"));
    for (const auto &s : a.build_resolve()) {
        std::cout << s << std::endl;
    }


    std::cout << "end test" << std::endl;

//    BasicRanges::build();

//    Benchmark::basic_ranges(std::cout);
//    std::cout << std::endl;
//    Benchmark::all_cases(std::cout);

//    std::cout << std::endl;
//    Benchmark::basic_ranges(std::cout);

    return 0;


//    BasicRanges::build();
//    AllCases::build();


//    std::vector<RawCode> test_cases;
//    {
//        AllCases::build();
//        std::vector<uint64_t> all_cases;
//        for (uint64_t head = 0; head < 16; ++head) {
//            for (const auto &range : AllCases::fetch()[head]) {
//                all_cases.emplace_back(head << 32 | range);
//            }
//        }
//        for (uint32_t i = 0; i < 100; ++i) {
//            test_cases.emplace_back(
//                RawCode::from_common_code(all_cases.at(i * 293344))
//            );
//        }
//    }
//    std::cout << "test size -> " << test_cases.size() << std::endl;


//    std::cout << "wait 3s" << std::endl;
//    sleep(3);

//    std::cout << "start benchmark" << std::endl;
//    auto start_time = clock();


//    auto fc = FastCal(RawCode::from_common_code("1a9bf0c"));
//    fc.solve();

//    {
//        auto fc = FastCal(RawCode::unsafe_create(0));
//        for (auto code : test_cases) {
//            fc.set_root(code);
//            fc.solve();
//        }
//    }


//    auto start_time = clock();

//    auto a = Analyse(RawCode::from_common_code("1a9bf0c"));
//    auto a = Analyse(RawCode::from_common_code("4fea134"));
//    auto a = Analyse(RawCode::from_common_code("A5D3AF"));


//    a.build();
//    auto ret = a.build_until([](uint64_t code) {
//        return ((code >> (3 * 0xD)) & 0b111) == B_2x2;
//    });
//    auto ret = a.build_resolve();
//    for (const auto &r : ret) {
//        std::cout << r << std::endl;
//    }
//    std::cout << ret.size() << std::endl;


//    std::cerr << (clock() - start_time) * 1000000 / CLOCKS_PER_SEC << "us" << std::endl;


//    start_time = clock();
//    auto svg_ret = a.backtrack(ret);
//    std::cerr << (clock() - start_time) * 1000000 / CLOCKS_PER_SEC << "us" << std::endl;


//    auto g = Graph();
//    auto svg_data = g.svg_demo(svg_ret);

//    std::cout << svg_data << std::endl;


//    std::cout << a.layer_export(81).size() << std::endl;
//    std::cout << a.layer_export()[81].size() << std::endl;


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


//    std::cerr << (clock() - start_time) / CLOCKS_PER_SEC << "s" << std::endl;
//    std::cerr << (clock() - start_time) * 1000 / CLOCKS_PER_SEC << "ms" << std::endl;
//    std::cerr << (clock() - start_time) * 1000000 / CLOCKS_PER_SEC << "us" << std::endl;

//    std::cout << "complete benchmark" << std::endl;

//    pause();

    return 0;
}
