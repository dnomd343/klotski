#include "common.h"
#include "all_cases.h"

std::mutex AllCases::building;
bool AllCases::available = false;
std::vector<uint32_t> AllCases::data[];

AllCases::Status AllCases::status() { // get all cases status
    if (AllCases::available) {
        return AVAILABLE; // all cases already built
    }
    if (!AllCases::building.try_lock()) { // fail to lock mutex
        return BUILDING; // another thread working
    }
    AllCases::building.unlock(); // release mutex
    return NO_INIT;
}

const std::vector<uint32_t> (*AllCases::fetch())[16] { // get all cases content
    if (status() != AVAILABLE) {
        AllCases::build(); // all cases initialize
    }
    return &AllCases::data; // return const ptr
}

void AllCases::build() { // ensure that all cases available
    if (!AllCases::available) {
        if (AllCases::building.try_lock()) { // mutex lock success
            build_data(); // start build process
            AllCases::available = true; // set available flag
        } else {
            AllCases::building.lock(); // blocking waiting
        }
        AllCases::building.unlock();
    }
}

void AllCases::build_data() { // find all cases
    for (uint32_t head = 0; head < 16; ++head) { // address of 2x2 block
        if ((head & 0b11) == 0b11) {
            continue; // invalid 2x2 address
        }
        /// head -> 0/1/2 / 4/5/6 / 8/9/10 / 12/13/14
        data[head].reserve(ALL_CASES_SIZE[head]); // memory pre-allocated
        for (auto const &range : *BasicRanges::fetch()) { // check base on 2x2 address and range
            if (Common::check_case(head, range)) {
                data[head].emplace_back(Common::range_reverse(range)); // found valid case
            }
        }
    }
}
