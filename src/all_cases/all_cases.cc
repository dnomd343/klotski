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


//int32_t AllCases::check_case(uint32_t head, uint32_t range) { // whether the head and range is valid
//
//    uint32_t mask = 0b110011 << head; // fill 2x2 block
//
//    for (int addr = 0; range; range >>= 2) { // traverse every 2-bits
//
//        while (mask >> addr & 0b1) {
//            ++addr; // search next not filled block
//        }
//
//        switch (range & 0b11) {
//            case 0b00: // space block
//            case 0b11: // 1x1 block
//                mask |= 0b1 << addr; // fill 1x1 block
//                break;
//            case 0b10: // 2x1 block
//                if (addr > 15 || mask >> (addr + 4) & 0b1) { // invalid address
//                    return false;
//                }
//                mask |= 0b10001 << addr; // fill 2x1 block
//                break;
//            case 0b01: // 1x2 block
//                if ((addr & 0b11) == 0b11 || mask >> (addr + 1) & 0b1) { // invalid address
//                    return false;
//                }
//                mask |= 0b11 << addr; // fill 1x2 block
//                break;
//        }
//    }
//
//    return true; // valid case
//
//}

uint32_t AllCases::check_case(uint32_t head, uint32_t range) { // whether the head and range is valid

    uint32_t num = 1;

    uint32_t mask = 0b110011 << head; // fill 2x2 block

    for (int addr = 0; range; range >>= 2, ++num) { // traverse every 2-bits

        while (mask >> addr & 0b1) {
            ++addr; // search next not filled block
        }

        switch (range & 0b11) {
            case 0b00: // space block
            case 0b11: // 1x1 block
                mask |= 0b1 << addr; // fill 1x1 block
                break;
            case 0b10: // 2x1 block
                if (addr > 15 || mask >> (addr + 4) & 0b1) { // invalid address
                    return num;
                }
                mask |= 0b10001 << addr; // fill 2x1 block
                break;
            case 0b01: // 1x2 block
                if ((addr & 0b11) == 0b11 || mask >> (addr + 1) & 0b1) { // invalid address
                    return num;
                }
                mask |= 0b11 << addr; // fill 1x2 block
                break;
        }
    }

    return 0; // valid case

}

#include <iostream>

void AllCases::build_data() { // find all cases

    uint32_t sum = 0;

    for (uint32_t head = 0; head < 16; ++head) { // address of 2x2 block
        if ((head & 0b11) == 0b11) {
            continue; // invalid 2x2 address
        }
        /// head -> 0/1/2 / 4/5/6 / 8/9/10 / 12/13/14
        data[head].reserve(ALL_CASES_SIZE[head]); // memory pre-allocated

        uint32_t ret;

        auto br = BasicRanges::fetch();

        for (auto i = 0; i < br->size(); ++i) {

            auto rev = Common::range_reverse((*br)[i]);

            if ((ret = check_case(head, (*br)[i])) == 0) {
                ++sum;

//                printf("%08X\n", Common::range_reverse(br->at(i)));

            } else {

                auto delta = (uint32_t)1 << (32 - ret * 2);
                auto next = (rev & ~(delta - 1)) + delta;

                while (Common::range_reverse((*br)[++i]) < next);
                --i;

            }

        }

    }
    std::cout << sum << std::endl;

}
