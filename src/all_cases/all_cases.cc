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

//    auto range = Common::range_reverse(0xA5740000);

    // A     5     7     4
    // 10 10 01 01 01 11 01 00
    //                   !!
    // search start at
    // 10 10 01 01 01 11 10 ...

//    auto offset = check_case(0, range);
//
//    auto delta = (uint32_t)1 << (32 - check_case(0, range) * 2);
//
//    auto next_at_least = Common::range_reverse(range) + delta;
//
//    printf("next at least -> %08X\n", next_at_least);

    /// 4258152: 0xA573F000
    /// 4258153: 0xA5740000
    /// 4258154: 0xA5743000
    /// 4258155: 0xA574C000
    /// 4258156: 0xA5770000
    /// 4258157: 0xA5780000
    /// 4258158: 0xA5783000
    /// 4258159: 0xA578C000

//    auto br = BasicRanges::fetch();
//
//    uint32_t ret;
//    for (auto i = 4258152; i <= 4258159; ++i) {
//        printf("check: %08X (%d) -> ", Common::range_reverse(br->at(i)), i);
//
//        if ((ret = check_case(0, br->at(i))) == 0) {
//            std::cout << "ok" << std::endl;
//        } else {
//            std::cout << "error" << std::endl;
//
////            std::cout << "ret = " << ret << std::endl;
//
//            auto delta = (uint32_t)1 << (32 - ret * 2);
//            auto mask = 0xFFFFFFFF << (32 - ret * 2);
//            auto next_at_least = (Common::range_reverse(br->at(i)) & mask) + delta;
//
//            for (;;) {
//                ++i;
//                if (Common::range_reverse(br->at(i)) >= next_at_least) {
//                    break;
//                }
//                std::cout << "skip -> next" << std::endl;
//            }
//
//
//            printf("next at least: %08X\n", next_at_least);
//
//        }
//    }

    /// A    5    7    7    0    0    00
    /// 1010 0101 0111 0111 0000 0000 ...

//    return;


//    for (uint32_t head = 0; head < 16; ++head) { // address of 2x2 block
    for (uint32_t head = 0; head < 1; ++head) { // address of 2x2 block
        if ((head & 0b11) == 0b11) {
            continue; // invalid 2x2 address
        }
        /// head -> 0/1/2 / 4/5/6 / 8/9/10 / 12/13/14
        data[head].reserve(ALL_CASES_SIZE[head]); // memory pre-allocated
//        for (const auto &range : *BasicRanges::fetch()) { // check base on 2x2 address and range

//            printf("%08X -> ", Common::range_reverse(range));

//            if (check_case(head, range)) {
//                printf("ok\n");
//                data[head].emplace_back(Common::range_reverse(range)); // found valid case
//            } else {
//                printf("error\n");
//            }

//        }

        uint32_t ret;

        auto br = BasicRanges::fetch();

        for (auto i = 0; i < br->size(); ++i) {

            printf("%d: %08X -> ", i, Common::range_reverse(br->at(i)));

            if ((ret = check_case(0, br->at(i))) == 0) {
                printf("ok\n");
            } else {
                printf("error\n");

                auto delta = (uint32_t)1 << (32 - ret * 2);
                auto mask = 0xFFFFFFFF << (32 - ret * 2);
                auto next_at_least = (Common::range_reverse(br->at(i)) & mask) + delta;
                printf("next at least: %08X\n", next_at_least);

                for (;;) {
                    ++i;
                    if (Common::range_reverse(br->at(i)) >= next_at_least) {
                        break;
                    }
                    std::cout << "skip -> next" << std::endl;
                }

            }

        }



    }
}
