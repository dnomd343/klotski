#include "common.h"
#include "all_cases.h"

std::mutex AllCases::building;
bool AllCases::available = false;
std::vector<uint32_t> AllCases::data[];

const std::vector<uint32_t> (&AllCases::fetch())[16] { // get all cases content
    if (status() != AVAILABLE) {
        AllCases::build(); // all cases initialize
    }
    return AllCases::data; // return const ref
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
    const auto &basic_ranges = BasicRanges::fetch();
    for (uint32_t head = 0; head < 15; ++head) { // address of 2x2 block
        if ((head & 0b11) == 0b11) {
            ++head; // skip invalid address
        }
        /// head -> 0/1/2 / 4/5/6 / 8/9/10 / 12/13/14
        data[head].reserve(ALL_CASES_SIZE[head]); // memory pre-allocated
        /// head(4-bits) + basic ranges(32-bits) ==check==> release valid cases
        for (auto index = 0; index < basic_ranges.size(); ++index) {
            auto broken_offset = Common::check_range(head, basic_ranges[index]);
            if (broken_offset) { // case invalid
                auto delta = (uint32_t)1 << (32 - broken_offset * 2); // delta to next possible range
                ///         !! -> broken
                /// ( xx xx xx ) xx xx xx ... (reversed range)
                ///         +1   00 00 00 ... (delta)
                auto next_min = (Common::range_reverse(basic_ranges[index]) & ~(delta - 1)) + delta;
                while (Common::range_reverse(basic_ranges[++index]) < next_min); // located next range
                --index;
            } else {
                AllCases::data[head].emplace_back(
                    Common::range_reverse(basic_ranges[index]) // release valid cases
                );
            }
        }
    }
}
