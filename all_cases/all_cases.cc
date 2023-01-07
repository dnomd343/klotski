#include "all_cases.h"

#include <iostream>
#include <thread>
#include <unistd.h>

std::vector<uint32_t> AllCases::basic_ranges;

bool AllCases::basic_ranges_available = false;

std::mutex AllCases::basic_ranges_building;

void AllCases::build_basic_ranges() {

    std::cout << std::this_thread::get_id() << " enter build function" << std::endl;

    if (AllCases::basic_ranges_available) {
        std::cout << std::this_thread::get_id() << " data already built -> skip" << std::endl;
        return; // basic ranges already built
    }

    if (AllCases::basic_ranges_building.try_lock()) { // lock success -> not building

        std::cout << std::this_thread::get_id() << " try lock success -> start build process" << std::endl;

        AllCases::basic_ranges.emplace_back(0);
        AllCases::basic_ranges.emplace_back(1);
        AllCases::basic_ranges.emplace_back(2);
        sleep(2); // assume using a lot of time

        AllCases::basic_ranges_available = true; // enable available flag

        std::cout << std::this_thread::get_id() << " build complete -> continue" << std::endl;

    } else { // another thread building

        std::cout << std::this_thread::get_id() << " another thread building -> wait" << std::endl;
        AllCases::basic_ranges_building.lock(); // blocking wait

    }
    std::cout << std::this_thread::get_id() << " work complete -> unlock" << std::endl;
    AllCases::basic_ranges_building.unlock();


}
