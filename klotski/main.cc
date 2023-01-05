#include <iostream>
#include "all_cases.h"

int main() {

    auto a = AllCases();
    a.build_basic_ranges();

    auto br = a.get_basic_ranges();
    std::cout << br->size() << std::endl;

    return 0;
}
