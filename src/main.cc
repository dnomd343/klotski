#include <iostream>
#include "all_cases.h"
#include "basic_ranges.h"
#include "common_code.h"
#include "short_code.h"
#include "raw_code.h"

#include "core_demo.h"

#include <thread>

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


    next_step(0x0E58FC85FFEBC4DB, 0); // mask unset

    std::cout << CommonCode(RawCode(0x0E58FC85FFEBC4DB)).to_string() << std::endl;

    return 0;
}
