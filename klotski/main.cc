#include <iostream>
#include "all_cases.h"
#include "short_code.h"
#include "common_code.h"

int main() {

//    auto a = AllCases();
//    auto a = AllCases(AllCases::Build::BASIC_RANGES);
//    auto a = AllCases(AllCases::Build::ALL_CASES);

//    std::cout << "start getting basic ranges" << std::endl;
//    std::cout << "basic range: " << a.get_basic_ranges()->size() << std::endl;

//    std::cout << "start getting all cases" << std::endl;
//    for (const auto &temp : *a.get_all_cases()) {
//        std::cout << "  " << temp.size() << std::endl;
//    }


//    auto s = ShortCode();
//    auto s = ShortCode(ShortCode::Mode::NORMAL);
//    auto s = ShortCode(ShortCode::Mode::FAST);

//    s.speed_up(ShortCode::Mode::NORMAL);
//    s.speed_up(ShortCode::Mode::FAST);

//    printf("%d\n", s.zip_short_code(0x6EC0F8800));
//    printf("%09lX\n", s.unzip_short_code(14323231));

//    std::cout << ShortCode::code_to_string(14323231) << std::endl;
//    std::cout << ShortCode::code_from_string("EP4HZ") << std::endl;


//    std::cout << CommonCode::code_to_string(0x4FEA13400, true) << std::endl;
//    std::cout << CommonCode::code_to_string(0x4FEA13400) << std::endl;

//    printf("%09lX\n", CommonCode::code_from_string("4FEa134"));
//    printf("%09lX\n", CommonCode::code_from_string("1A9bf0C0"));


//    std::cout << (int)last_zero_num(0b1) << std::endl;
//    std::cout << (int)last_zero_num(0b1000) << std::endl;
//    std::cout << (int)last_zero_num(0) << std::endl;
//    std::cout << CommonCode::code_to_string(0, true) << std::endl;

    auto a = AllCases();
    std::vector<uint64_t> all_cases;
    for (int head = 0; head < 16; ++head) {
        auto prefix = (uint64_t)head << 32;
        for (auto const &range : (*a.get_all_cases())[head]) {
            all_cases.emplace_back(prefix | range);
        }
    }
//    std::cout << all_cases.size() << std::endl;
    for (auto const &code : all_cases) {
//        printf("%s\n", CommonCode::code_to_string(code).c_str());
        std::string code_str = CommonCode::code_to_string(code);
        if (CommonCode::code_from_string(code_str) != code) {
            std::cout << "ERROR: " << code_str << std::endl;
        }

        code_str = CommonCode::code_to_string(code, true);
        if (CommonCode::code_from_string(code_str) != code) {
            std::cout << "ERROR: " << code_str << std::endl;
        }
    }

    return 0;
}
