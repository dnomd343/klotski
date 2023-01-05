#include <iostream>
#include "all_cases.h"

int main() {

    auto a = AllCases();

    std::cout << "basic range size: " << a.get_basic_ranges()->size() << std::endl;

    for (const auto &temp : *a.get_all_cases()) {
        std::cout << temp.size() << std::endl;
    }

    return 0;
}
