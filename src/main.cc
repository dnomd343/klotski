#include <iostream>
#include "all_cases.h"
#include "basic_ranges.h"
#include "common_code.h"

//void get_status() {
//    switch (BasicRanges::basic_ranges_status()) {
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
//    switch (AllCases::all_cases_status()) {
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

//    get_status();
//    BasicRanges::build_basic_ranges();
//    get_status();
//    AllCases::build_all_cases();
//    get_status();
//
//    for (auto const &all_case : *AllCases::get_all_cases()) {
//        std::cout << "  " << all_case.size() << std::endl;
//    }
//
//    std::cout << BasicRanges::get_basic_ranges() << std::endl;
//    std::cout << AllCases::get_basic_ranges() << std::endl;


//    std::cout << CommonCode::check(0x123456789) << std::endl;
//    std::cout << CommonCode::check(0x4FEA13400) << std::endl;
//
//    // TODO: should we return a CommonCode object like String::new(...) in rust?
//    printf("%09lX\n", CommonCode::from_string("1A9bF0c0"));
//    std::cout << CommonCode::to_string(0x1A9BF0C00) << std::endl;
//    std::cout << CommonCode::to_string(0x1A9BF0C00, true) << std::endl;

    auto c = CommonCode::from_string("1A9bF0c0");
    std::cout << c.to_string(true) << std::endl;
    std::cout << c.to_string() << std::endl;
    printf("%09lX\n", c.unwrap());

    std::cout << CommonCode(0x1A9BF0C00).to_string() << std::endl;

    return 0;
}
