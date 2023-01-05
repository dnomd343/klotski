#include <iostream>
#include "all_cases.h"

int main() {

//    auto a = AllCases();
//    auto a = AllCases(AllCases::InitType::WITH_NOTHING);
//    auto a = AllCases(AllCases::InitType::WITH_BASIC_RANGES);
    auto a = AllCases(AllCases::InitType::WITH_ALL_CASES);

    std::cout << "start get basic ranges" << std::endl;
    std::cout << "basic range: " << a.get_basic_ranges()->size() << std::endl;

    std::cout << "start get all cases" << std::endl;
    for (const auto &temp : *a.get_all_cases()) {
        std::cout << temp.size() << std::endl;
    }

    return 0;
}
