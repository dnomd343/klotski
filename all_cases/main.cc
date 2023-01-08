#include <iostream>
#include "all_cases.h"
#include "basic_ranges.h"

#include <thread>

void get_status() {
    switch (BasicRanges::basic_ranges_status()) {
        case BasicRanges::NO_INIT:
            std::cout << "basic ranges no init" << std::endl;
            break;
        case BasicRanges::BUILDING:
            std::cout << "basic ranges building" << std::endl;
            break;
        case BasicRanges::AVAILABLE:
            std::cout << "basic ranges available" << std::endl;
            break;
    }

    switch (AllCases::all_cases_status()) {
        case AllCases::NO_INIT:
            std::cout << "all cases no init" << std::endl;
            break;
        case AllCases::BUILDING:
            std::cout << "all cases building" << std::endl;
            break;
        case AllCases::AVAILABLE:
            std::cout << "all cases available" << std::endl;
            break;
    }
}

int main() {

//    get_status();
//
//    std::thread t1(BasicRanges::build_basic_ranges);
//    std::thread t2(BasicRanges::build_basic_ranges);
//    std::thread t3(BasicRanges::build_basic_ranges);
//    std::thread t(get_status);
//    t1.join();
//    t2.join();
//    t3.join();
//    t.join();
//
//    get_status();
//
//    std::cout << BasicRanges::get_basic_ranges()->size() << std::endl;


    get_status();

    std::thread t1(AllCases::build_all_cases);
    std::thread t2(AllCases::build_all_cases);
    std::thread t3(AllCases::build_all_cases);
    std::thread t(get_status);
    t1.join();
    t2.join();
    t3.join();
    t.join();

//    AllCases::build_all_cases();

    get_status();

    for (auto const &all_case : *AllCases::get_all_cases()) {
        std::cout << "  " << all_case.size() << std::endl;
    }

    std::cout << BasicRanges::get_basic_ranges() << std::endl;
    std::cout << AllCases::get_basic_ranges() << std::endl;

    return 0;
}
