#include "common.h"
#include "all_cases.h"

std::mutex AllCases::all_cases_building;
bool AllCases::all_cases_available = false;
std::vector<uint32_t> AllCases::all_cases[];

const std::vector<uint32_t> (*AllCases::get_all_cases())[16] { // get const ptr of all cases
    if (all_cases->empty()) {
        build_all_cases(); // all cases initialize
    }
    return &all_cases; // return ptr
}

void AllCases::build_all_cases() { // build all cases
    if (AllCases::all_cases_available) {
        return; // all cases already built
    }
    if (AllCases::all_cases_building.try_lock()) { // lock success -> start building
        /// head -> 0/1/2 / 4/5/6 / 8/9/10 / 12/13/14
        for (uint32_t head = 0; head < 16; ++head) { // address of 2x2 block
            if ((head & 0b11) == 0b11) {
                continue; // invalid 2x2 address
            }
            for (uint32_t const &range : *get_basic_ranges()) { // check base on 2x2 address and range
                if (Common::check_case(head, range)) {
                    all_cases[head].emplace_back(Common::range_reverse(range)); // found valid case
                }
            }
        }
        AllCases::all_cases_available = true; // set available flag
    } else { // another thread building
        AllCases::all_cases_building.lock(); // blocking waiting
    }
    AllCases::all_cases_building.unlock();
}

AllCases::Status AllCases::all_cases_status() { // get all cases status
    if (all_cases_available) {
        return AVAILABLE; // all cases already built
    }
    if (!all_cases_building.try_lock()) { // fail to lock mutex -> another thread working
        return BUILDING;
    }
    all_cases_building.unlock(); // release mutex
    return NO_INIT;
}
