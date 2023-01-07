#include <iostream>
#include "all_cases.h"

int main() {

    std::cout << AllCases::basic_ranges.size() << std::endl;
    AllCases::build_basic_ranges();
    std::cout << AllCases::basic_ranges.size() << std::endl;

    return 0;
}
