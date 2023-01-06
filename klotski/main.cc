#include <iostream>
//#include "all_cases.h"
#include "short_code.h"
#include "common_code.h"

int main() {

//    auto a = AllCases();
//    auto a = AllCases(AllCases::InitType::WITH_NOTHING);
//    auto a = AllCases(AllCases::InitType::WITH_BASIC_RANGES);
//    auto a = AllCases(AllCases::InitType::WITH_ALL_CASES);

//    std::cout << "start get basic ranges" << std::endl;
//    std::cout << "basic range: " << a.get_basic_ranges()->size() << std::endl;

//    std::cout << "start get all cases" << std::endl;
//    for (const auto &temp : *a.get_all_cases()) {
//        std::cout << temp.size() << std::endl;
//    }

//    auto s = ShortCode();
//    auto s = ShortCode(ShortCode::Mode::NORMAL);
//    auto s = ShortCode(ShortCode::Mode::FAST);

//    s.speed_up(ShortCode::Mode::FAST);
//    std::cout << s.all_cases_list.size() << std::endl;
//    std::cout << s.all_cases_dict.size() << std::endl;

//    printf("%d\n", s.fast_encode(0x6EC0F8800));
//    printf("%09lX\n", s.fast_decode(14323231));

//    s.speed_up(ShortCode::Mode::NORMAL);
//    std::cout << s.basic_ranges.size() << std::endl;

//    auto s = ShortCode();
//    s.speed_up(ShortCode::Mode::NORMAL); // enter normal mode first
//
//    printf("%d\n", s.tiny_encode(0x6EC0F8800));
//    printf("%09lX\n", s.tiny_decode(14323231));


//    if (CommonCode::check(0x4FEA13400)) {
//        std::cout << "true" << std::endl;
//    } else {
//        std::cout << "false" << std::endl;
//    }

//    uint32_t sum = 0;
    for (uint32_t head = 0; head < 16; ++head) {
        auto prefix = (uint64_t)head << 32;
        for (uint64_t range = 0; range < 0x100000000; ++range) {
            uint64_t code = prefix | range;
            if (CommonCode::check(code)) {
                printf("%09lX\n", code);
//                ++sum;
            }
//            if (range % 0x1000000 == 0) {
//                std::cout << range / 0x1000000 << std::endl;
//            }
        }
    }
//    std::cout << "sum: " << sum << std::endl;

//    uint64_t error_code = 0x02EFFF7C0;
//    uint64_t error_code = 0x07B4EFC00;

//    if (CommonCode::check(error_code)) {
//        std::cout << "true" << std::endl;
//    } else {
//        std::cout << "false" << std::endl;
//    }

    return 0;
}
