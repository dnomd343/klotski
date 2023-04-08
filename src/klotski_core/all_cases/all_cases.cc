#include "common.h"
#include "all_cases.h"

namespace klotski {

using Common::check_range;
using Common::range_reverse;

/// static variable initialize
std::mutex AllCases::building_;
bool AllCases::available_ = false;
std::vector<uint32_t> AllCases::data_[];

const std::vector<uint32_t> (&AllCases::fetch())[16] {
    if (status() != AVAILABLE) {
        build();
    }
    return data_; // return const reference
}

AllCases::Status AllCases::status() noexcept {
    if (available_) {
        return AVAILABLE; // data already built
    }
    if (!building_.try_lock()) { // fail to lock mutex
        return BUILDING; // another thread working
    }
    building_.unlock(); // release mutex
    return NOT_INIT;
}

void AllCases::build() { // ensure that data is available
    if (!available_) {
        if (building_.try_lock()) { // mutex lock success
            build_data(); // start build process
            available_ = true;
        } else {
            building_.lock(); // blocking waiting
        }
        building_.unlock();
    }
}

std::vector<uint64_t> AllCases::release() {
    std::vector<uint64_t> data;
    data.reserve(ALL_CASES_SIZE_SUM); // memory pre-allocated
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : fetch()[head]) {
            data.emplace_back(head << 32 | range); // release common codes
        }
    }
    return data;
}

/// Search all possible layouts based on basic-ranges.
void AllCases::build_data() {
    const auto &basic_ranges = BasicRanges::fetch();
    for (uint32_t head = 0; head < 15; ++head) { // address of 2x2 block
        /// head -> 0/1/2 / 4/5/6 / 8/9/10 / 12/13/14
        if ((head & 0b11) == 0b11) {
            ++head; // skip invalid address
        }
        data_[head].reserve(ALL_CASES_SIZE[head]); // memory pre-allocated

        /// head(4-bit) + basic-range(32-bit) --check--> valid cases
        for (uint32_t index = 0; index < basic_ranges.size(); ++index) {
            auto broken_offset = check_range(head, basic_ranges[index]);
            if (broken_offset) { // case invalid
                auto delta = (uint32_t)1 << (32 - broken_offset * 2); // delta to next possible range
                ///         !! -> broken
                /// ( xx xx xx ) xx xx xx ... (reversed range)
                ///         +1   00 00 00 ... (delta)
                auto at_least = (range_reverse(basic_ranges[index]) & ~(delta - 1)) + delta;
                while (range_reverse(basic_ranges[++index]) < at_least); // located next range
                --index;
                continue;
            }
            data_[head].emplace_back(
                range_reverse(basic_ranges[index]) // release valid cases
            );
        }
    }
}

} // namespace klotski
