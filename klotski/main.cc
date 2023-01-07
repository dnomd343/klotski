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

    printf("%09lX\n", CommonCode::code_from_string("4FEa134"));
    printf("%09lX\n", CommonCode::code_from_string("1A9bf0C0"));

    return 0;
}
