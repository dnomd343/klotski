#include <iostream>
//#include "all_cases.h"
#include "short_code.h"

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

    auto s = ShortCode();

    s.speed_up();
//    std::cout << s.all_cases_list.size() << std::endl;
//    std::cout << s.all_cases_dict.size() << std::endl;

    printf("%d\n", s.fast_encode(0x6EC0F8800));
    printf("%09lX\n", s.fast_decode(14323231));

    return 0;
}
