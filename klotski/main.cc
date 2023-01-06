#include <iostream>
#include "all_cases.h"
#include "short_code.h"
//#include "common_code.h"

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

//    s.speed_up(ShortCode::Mode::NORMAL);
//    s.speed_up(ShortCode::Mode::FAST);

//    printf("%d\n", s.zip_short_code(0x6EC0F8800));
//    printf("%09lX\n", s.unzip_short_code(14323231));


//    std::cout << "preparing..." << std::endl;
//    std::vector<uint64_t> all_cases;
//    auto a = AllCases(AllCases::ALL_CASES);
//    for (int head = 0; head < 16; ++head) {
//        uint64_t prefix = (uint64_t)head << 32;
//        for (const auto &range : (*a.get_all_cases())[head]) {
//            all_cases.emplace_back(prefix | range);
//        }
//    }
//
////    auto s = ShortCode(ShortCode::Mode::NORMAL);
//    auto s = ShortCode(ShortCode::Mode::FAST);
//    std::cout << "start working" << std::endl;
//
//    for (auto short_code = 0; short_code < all_cases.size(); ++short_code) {
//        uint64_t common_code = all_cases[short_code];
//
//        if (short_code != s.zip_short_code(common_code)) {
//            printf("ERROR: zip %d\n", short_code);
//        }
//        if (common_code != s.unzip_short_code(short_code)) {
//            printf("ERROR: unzip %09lX\n", common_code);
//        }
//
//        if (short_code % 2000000 == 0) {
////        if (short_code % 10000 == 0) {
//            std::cout << ((float)short_code / (float)all_cases.size() * 100) << "%" << std::endl;
//        }
//    }
//    std::cout << "complete verify" << std::endl;


    for (uint32_t short_code = 0; short_code < 29334498; ++short_code) {
        if (short_code != ShortCode::code_from_string(ShortCode::code_to_string(short_code))) {
            std::cout << "ERROR: " << short_code << std::endl;
        }
    }

//    std::cout << ShortCode::code_to_string(14323231) << std::endl;
//    std::cout << ShortCode::code_from_string("EP4HZ") << std::endl;

    return 0;
}
