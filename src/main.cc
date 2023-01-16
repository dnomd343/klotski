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

//#include "core_demo.h"
//#include "basic_ranges_demo.h"

#include <thread>
#include <algorithm>
#include <unistd.h>

//void get_status() {
//    switch (BasicRanges::status()) {
//        case BasicRanges::NO_INIT:
//            std::cout << "basic ranges no init" << std::endl;
//            break;
//        case BasicRanges::BUILDING:
//            std::cout << "basic ranges building" << std::endl;
//            break;
//        case BasicRanges::AVAILABLE:
//            std::cout << "basic ranges available" << std::endl;
//            break;
//    }
//
//    switch (AllCases::status()) {
//        case AllCases::NO_INIT:
//            std::cout << "all cases no init" << std::endl;
//            break;
//        case AllCases::BUILDING:
//            std::cout << "all cases building" << std::endl;
//            break;
//        case AllCases::AVAILABLE:
//            std::cout << "all cases available" << std::endl;
//            break;
//    }
//}

//void release(uint64_t code, uint64_t mask) {
//    std::cout << RawCode(code).dump_case();
//    std::cout << "~~~~~~~" << std::endl;
//    for (int n = 0; n < 20; ++n, mask >>= 3) {
//        std::cout << "+."[!(mask & 0b111)] << " \n"[!(~n & 0b11)];
//    }
//    std::cout << std::endl;
//}

int main() {

//    printf("%p\n", BasicRanges::build);
//    printf("%p\n", AllCases::build);
//
//    printf("%p\n", BasicRanges::status);
//    printf("%p\n", AllCases::status);
//
//    printf("%p\n", BasicRanges::fetch);
//    printf("%p\n", AllCases::fetch);
//
//    get_status();
//    BasicRanges::build();
//    get_status();
//    AllCases::build();
//    get_status();
//
//    std::cout << BasicRanges::fetch()->size() << std::endl;
//
//    uint32_t sum = 0;
//    for (auto const &all_case : *AllCases::fetch()) {
//        sum += all_case.size();
//        std::cout << "  " << all_case.size() << std::endl;
//    }
//    std::cout << sum << std::endl;
//
//    std::cout << AllCases::fetch() << std::endl;
//    std::cout << AllCases::BasicRanges::fetch() << std::endl;
//    std::cout << BasicRanges::fetch() << std::endl;


//    std::cout << CommonCode::check(0x123456789) << std::endl;
//    std::cout << CommonCode::check(0x4FEA13400) << std::endl;
//
//    printf("%09lX\n", CommonCode("1A9bF0c0").unwrap());
//    std::cout << CommonCode(0x1A9BF0C00).to_string() << std::endl;
//    std::cout << CommonCode(0x1A9BF0C00).to_string(true) << std::endl;
//
//    auto c = CommonCode("4Fea13400");
//    std::cout << c.to_string(true) << std::endl;
//    std::cout << c.to_string() << std::endl;
//    printf("%09lX\n", c.unwrap());


//    std::cout << ShortCode::check_mode() << std::endl;
//
//    std::cout << "start NORMAL speed up" << std::endl;
//    std::thread t1(ShortCode::speed_up, ShortCode::NORMAL);
//    std::thread t2(ShortCode::speed_up, ShortCode::NORMAL);
//    t1.join();
//    t2.join();
//    std::cout << "NORMAL speed up complete" << std::endl;
//
//    std::cout << ShortCode::check_mode() << std::endl;
//
//    std::cout << "start FAST speed up" << std::endl;
//    std::thread t3(ShortCode::speed_up, ShortCode::FAST);
//    std::thread t4(ShortCode::speed_up, ShortCode::FAST);
//    t3.join();
//    t4.join();
//    std::cout << "FAST speed up complete" << std::endl;
//
//    std::cout << ShortCode::check_mode() << std::endl;


//    auto s = ShortCode(14323231);
////    auto s = ShortCode(14323231, ShortCode::NORMAL);
////    auto s = ShortCode(14323231, ShortCode::FAST);
//    std::cout << s.unwrap() << std::endl;
//    std::cout << s.to_string() << std::endl;
//    std::cout << ShortCode("EP4HZ", ShortCode::NORMAL).unwrap() << std::endl;
//    std::cout << ShortCode("eP4hZ", ShortCode::FAST).to_string() << std::endl;


//    ShortCode::speed_up(ShortCode::FAST);
//    std::cout << ShortCode(14323231).to_common_code().to_string() << std::endl;
//    std::cout << ShortCode(CommonCode(0x6EC0F8800)).to_string() << std::endl;
//
////    ShortCode::speed_up(ShortCode::FAST);
//    std::cout << ShortCode(14323231).to_common_code().to_string() << std::endl;
//    std::cout << ShortCode(CommonCode(0x6EC0F8800)).to_string() << std::endl;

//    std::cout << ShortCode(CommonCode(0x6EC0F8800), ShortCode::NORMAL).to_string() << std::endl;
//    std::cout << ShortCode(CommonCode(0x6EC0F8800), ShortCode::FAST).to_string() << std::endl;

//    std::cout << CommonCode(0x6EC0F8800).to_short_code().to_string() << std::endl;

//    std::cout << CommonCode(ShortCode(14323231)).to_string() << std::endl;
//    std::cout << ShortCode(14323231).to_common_code().to_string() << std::endl;
//
//    std::cout << ShortCode(CommonCode(0x6EC0F8800)).to_string() << std::endl;
//    std::cout << CommonCode(0x6EC0F8800).to_short_code().to_string() << std::endl;


//    auto r = RawCode(0x0E58FC85FFEBC4DB);
//    printf("%015lX\n", r.unwrap());
//
//    std::cout << r.to_common_code().to_string() << std::endl;
//    printf("%015lX\n", RawCode(CommonCode(0x4FEA13400)).unwrap());

//    auto r = RawCode(0x0E58FC85FFEBC4DB);
//    auto r = RawCode(0x0E58FC85FFEBC4DD); // invalid raw code -> only for test
//    std::cout << r.dump_case() << std::endl;

//    std::cout << CommonCode(RawCode(0x0E58FC85FFEBC4DB)).to_string() << std::endl;


//    auto c = Core();

//    auto raw_code = RawCode(CommonCode("4fea134")).unwrap();
//    auto raw_code = RawCode(CommonCode("1A9bf0c")).unwrap();
//    c.next_step(raw_code, release);

//    std::vector<uint64_t> all_cases_raw;
//    for (int head = 0; head < 16; ++head) {
//        uint64_t prefix = (uint64_t)head << 32;
//        for (auto const &range : (*AllCases::fetch())[head]) {
//            all_cases_raw.emplace_back(
//                RawCode(CommonCode::unsafe_create(prefix | range)).unwrap()
//            );
//        }
//    }

    ShortCode::speed_up(ShortCode::FAST);
    AllCases::build();

    std::vector<uint64_t> all_cases;
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : BasicRanges::fetch()) {
            all_cases.emplace_back(head << 32 | range);
        }
    }

//    std::cout << "start benchmark" << std::endl;
    auto start_time = clock();

//    auto raw_code = RawCode(CommonCode("4fea134")).unwrap();
//    for (int i = 0; i < 100000000; ++i) {
//        c.next_step(raw_code);
//    }

//    std::vector<uint32_t> steps;
//    for (auto const &raw_code : all_cases_raw) {
//        steps.emplace_back(c.next_step(raw_code));
//    }
//    std::sort(steps.begin(), steps.end());
//    std::cout << steps[0] << std::endl;
//    std::cout << steps[steps.size() - 1] << std::endl;
//    for (int i = 0; i < all_cases_raw.size(); ++i) {
//        if (steps[i] == 0) {
//        if (steps[i] == 68) { // max next steps
//            std::cout << RawCode(all_cases_raw[i]).dump_case();
//            std::cout << CommonCode(RawCode(all_cases_raw[i])).to_string() << std::endl << std::endl;
//        }
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

//    BasicRanges::build();
//    const auto &br = BasicRanges::fetch();
//    br.push_back(123);
//    std::cout << "size: " << BasicRanges::fetch().size() << std::endl;
//    std::cout << "size: " << br.size() << std::endl;
//    for (const auto &range : *BasicRanges::fetch()) {
//        printf("%08X\n", range);
//    }

//    AllCases::build();
//    for (auto head = 0; head < 16; ++head) {
//        uint64_t prefix = (uint64_t)head << 32;
//        for (const auto &range : AllCases::fetch()[head]) {
//            printf("%09lX\n", prefix | range);
//        }
//    }

    for (uint32_t i = 0; i < 29334498; ++i) {
//        if (ShortCode::fast_encode(ShortCode::fast_decode(i)) != i) {
//            std::cout << "error" << std::endl;
//        }
//        ShortCode::fast_encode_legacy(i);
//        ShortCode::fast_decode(i);
        ShortCode::fast_encode(all_cases[i]);
    }

//    printf("%09lX\n", ShortCode::tiny_decode(14323231));
//    std::cout << ShortCode::tiny_encode_demo(0x6EC0F8800) << std::endl;

//    auto br = BasicRanges::fetch();
//    for (auto &range : br) {
//        range = Common::range_reverse(range);
//    }
    /// 1017983: 0FFFFFFF
    /// 1017984: 10000000
    /// 1017985: 1000000C
//    int sum = 0;
//    printf("    ");
//    for (uint32_t prefix = 0; prefix < 0x1000; ++prefix) {
//        printf("%7td, ", std::lower_bound(br.begin(), br.end(), prefix << 20) - br.begin());
//        if (sum++ % 8 == 7) {
//            printf("\n    ");
//        }
//    }

//    for (int head = 0; head < 16; ++head) {
//        auto &a = AllCases::fetch()[head];
//        printf("\n/// --------------------------------- 0x%X ---------------------------------\n    ", head);
//        for (uint32_t prefix = 0; prefix < 0x1000; ++prefix) {
//            printf("%7td, ", std::lower_bound(a.begin(), a.end(), prefix << 20) - a.begin());
//            if (sum++ % 8 == 7 and prefix != 0xFFF) {
//                printf("\n    ");
//            }
//        }
//    }
//    printf("\n");

//    printf("%09lX\n", ShortCode::fast_decode(14323231));
//    std::cout << ShortCode::fast_encode(0x6EC0F8800) << std::endl;

//    std::cerr << (clock() - start_time) * 1000 / CLOCKS_PER_SEC << "ms" << std::endl;
    std::cerr << (clock() - start_time) * 1000000 / CLOCKS_PER_SEC << "us" << std::endl;
//    std::cout << "complete benchmark" << std::endl;

//    pause();

    return 0;
}
