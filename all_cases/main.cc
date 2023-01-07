#include <iostream>
#include "all_cases.h"

#include <thread>
#include <unistd.h>

int main() {

    std::cout << AllCases::basic_ranges.size() << std::endl;

    std::thread t1(AllCases::build_basic_ranges);
    usleep(100);
    std::thread t2(AllCases::build_basic_ranges);
    usleep(100);
    std::thread t3(AllCases::build_basic_ranges);
    usleep(100);

    t1.join();
    t2.join();
    t3.join();

    AllCases::build_basic_ranges();

    std::cout << AllCases::basic_ranges.size() << std::endl;

    return 0;
}
