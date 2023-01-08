#include <iostream>
#include "all_cases.h"
#include "basic_ranges.h"

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

    get_status();
    BasicRanges::build_basic_ranges();
    get_status();
    AllCases::build_all_cases();
    get_status();

    for (auto const &all_case : *AllCases::get_all_cases()) {
        std::cout << "  " << all_case.size() << std::endl;
    }

    std::cout << BasicRanges::get_basic_ranges() << std::endl;
    std::cout << AllCases::get_basic_ranges() << std::endl;

    return 0;
}
