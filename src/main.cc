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
#include "offset/all_cases_offset.h"

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

//std::vector<uint64_t> all_cases;
//
//void short_code_verify(uint32_t start, uint32_t end) {
//    for (uint32_t short_code = start; short_code < end; ++short_code) {
//        auto common_code = all_cases[short_code]; // correct result
//
//        if (ShortCode(short_code).to_common_code().unwrap() != common_code) {
//            std::cout << "Error decode: " << ShortCode(short_code) << std::endl;
//        }
//
//        if (ShortCode(CommonCode(common_code)).unwrap() != short_code) {
//            std::cout << "Error encode: " << ShortCode(short_code) << std::endl;
//        }
//    }
//    std::cout << "[" << start << ", " << end << ")" << std::endl;
//}

int main() {

//    AllCases::build();
//
//    for (uint64_t head = 0; head < 16; ++head) {
//        for (const auto &range : AllCases::fetch()[head]) {
//            all_cases.emplace_back(head << 32 | range);
//        }
//    }
//
//    ShortCode::speed_up(ShortCode::NORMAL);
//    std::cout << "start verify" << std::endl;
//
    auto start_time = clock();
//
//    std::thread tasks[16];
//
//    for (int head = 0; head < 16; ++head) {
//        uint32_t start = ALL_CASES_OFFSET[head];
//        uint32_t end = start + AllCases::fetch()[head].size();
//        tasks[head] = std::thread(short_code_verify, start, end);
//    }
//
//    for (auto &t : tasks) {
//        t.join();
//    }
//
//    std::cout << "verify complete" << std::endl;
//    return 0;



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

//    ShortCode::speed_up(ShortCode::FAST);
//    AllCases::build();
//    BasicRanges::build();

//    std::vector<uint64_t> all_cases;
//    for (uint64_t head = 0; head < 16; ++head) {
//        for (const auto &range : AllCases::fetch()[head]) {
//            all_cases.emplace_back(head << 32 | range);
//        }
//    }
//    std::cout << "test data size: " << all_cases.size() << std::endl;

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

//    for (uint32_t i = 1000000; i < 1010000; ++i) {
//        ShortCode::tiny_decode(i);
//        ShortCode::tiny_encode(all_cases[i]);
//    }

//    printf("%09lX\n", ShortCode::tiny_decode(14323231));
//    std::cout << ShortCode::tiny_encode(0x6EC0F8800) << std::endl;

//    std::cout << ShortCode(14323231).to_string() << std::endl;
//    std::cout << ShortCode::from_string("EP4HZ").unwrap() << std::endl;

//    std::cout << ShortCode("EP4HZ") << std::endl;
//    std::cout << ShortCode(14323231) << std::endl;

//    printf("%09lX\n", ShortCode::fast_decode(14323231));
//    std::cout << ShortCode::fast_encode(0x6EC0F8800) << std::endl;

//    std::cout << sizeof(ShortCode) << std::endl;

//    int sum = 0;
//    for (uint64_t common_code = 0; common_code < 0x1000000000; ++common_code) {
//        if (CommonCode::check(common_code)) {
//            printf("%09lX\n", common_code);
//            ++sum;
//        }

//        if (common_code % 0x10000000 == 0) {
//            std::cerr << common_code / 0x10000000 << std::endl;
//        }

//        if (CommonCode::check(common_code) != CommonCode::check_demo(common_code)) {
//            printf("%09lX\n", common_code);
//        }
//    }
//    std::cout << "sum = " << sum << std::endl;

//    std::cout << CommonCode::check_demo(0x4FEA13400) << std::endl;
//    std::cout << CommonCode::check_demo(0x0000011CE) << std::endl;
//    std::cout << CommonCode::check_demo(0x02ED67000) << std::endl;

//    std::cout << CommonCode("1a9bf0C0").to_string() << std::endl;
//    std::cout << CommonCode(0x4FEA13400).to_string() << std::endl;

//    printf("%09lX\n", CommonCode::from_string("1a9bf0c").unwrap());
//    std::cout << CommonCode("4feA134") << std::endl;

    std::vector<uint64_t> all_cases;
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : AllCases::fetch()[head]) {
            all_cases.emplace_back(head << 32 | range);
        }
    }
    std::cout << "test size: " << all_cases.size() << std::endl;

    for (const auto &common_code : all_cases) {
        if (RawCode::compact(RawCode::extract(common_code)) != common_code) {
            std::cout << "Error: " << CommonCode(common_code) << std::endl;
        }
    }

//    std::cout << CommonCode(RawCode(RawCode::extract(0x4FEA13400))) << std::endl;

//    std::cerr << (clock() - start_time) * 1000 / CLOCKS_PER_SEC << "ms" << std::endl;
    std::cerr << (clock() - start_time) * 1000000 / CLOCKS_PER_SEC << "us" << std::endl;
//    std::cout << "complete benchmark" << std::endl;

//    pause();

    return 0;
}
