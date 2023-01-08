#include <iostream>
//#include "all_cases.h"
#include "basic_ranges.h"

#include <thread>

int main() {
    std::thread t1(BasicRanges::build_basic_ranges);
    std::thread t2(BasicRanges::build_basic_ranges);
    std::thread t3(BasicRanges::build_basic_ranges);
    t1.join();
    t2.join();
    t3.join();

//    BasicRanges::build_basic_ranges();
    std::cout << BasicRanges::get_basic_ranges()->size() << std::endl;

    return 0;
}
