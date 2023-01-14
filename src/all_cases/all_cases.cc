#include "common.h"
#include "all_cases.h"

std::mutex AllCases::building;
bool AllCases::available = false;
std::vector<uint32_t> AllCases::data[];

const std::vector<uint32_t> (*AllCases::fetch())[16] { // get all cases content
    if (status() != AVAILABLE) {
        AllCases::build(); // all cases initialize
    }
    return &AllCases::data; // return const ptr
}

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
    auto basic_ranges = BasicRanges::fetch();
    for (uint32_t head = 0; head < 15; ++head) { // address of 2x2 block
        if ((head & 0b11) == 0b11) {
            ++head; // skip invalid address
        }
        /// head -> 0/1/2 / 4/5/6 / 8/9/10 / 12/13/14
        data[head].reserve(ALL_CASES_SIZE[head]); // memory pre-allocated
        for (auto index = 0; index < basic_ranges->size(); ++index) {
            uint32_t broken = check_case(head, (*basic_ranges)[index]); // check and get broken address
            auto range_rev = Common::range_reverse((*basic_ranges)[index]); // reversed range
            if (broken) { // invalid case
                auto delta = (uint32_t)1 << (32 - broken * 2); // this <--delta--> next possible range
                auto min_next = (range_rev & ~(delta - 1)) + delta;
                while (Common::range_reverse((*basic_ranges)[++index]) < min_next); // located next range
                --index;
            } else {
                AllCases::data[head].emplace_back(range_rev); // release valid cases
            }
        }
    }
}

int AllCases::check_case(uint32_t head, uint32_t range) { // check the head and range
    constexpr uint32_t M_1x1 = 0b1;
    constexpr uint32_t M_1x2 = 0b11;
    constexpr uint32_t M_2x1 = 0b10001;
    constexpr uint32_t M_2x2 = 0b110011;

    int block_num = 1;
    uint32_t cache = M_2x2 << head; // fill 2x2 block
    for (int addr = 0; range; range >>= 2, ++block_num) { // traverse every 2-bits
        while (cache >> addr & 0b1) {
            ++addr; // search next not filled block
        }
        switch (range & 0b11) {
            case 0b00: // space
            case 0b11: // 1x1 block
                cache |= M_1x1 << addr; // fill space or 1x1 block
                break;
            case 0b10: // 2x1 block
                if (addr > 15 || cache >> (addr + 4) & 0b1) { // invalid address
                    return block_num; // broken block number
                }
                cache |= M_2x1 << addr; // fill 2x1 block
                break;
            case 0b01: // 1x2 block
                if ((addr & 0b11) == 0b11 || cache >> (addr + 1) & 0b1) { // invalid address
                    return block_num; // broken block number
                }
                cache |= M_1x2 << addr; // fill 1x2 block
                break;
        }
    }
    return 0; // pass
}
