#include <BS_thread_pool.hpp>

#include "codec.h"

void head_parallel(std::function<void(uint64_t head)> &&func) {
    BS::thread_pool pool;
    // TODO: skip invalid head
    pool.detach_sequence(0, 16, [func = std::move(func)](const uint64_t head) {
        if (head == 3 || head == 7 || head == 11 || head == 15) {
            return;
        }
        func(head);
    });
    pool.wait();
}

std::vector<uint64_t> all_common_codes() {
    // TODO: using `std::ranges`
    std::vector<uint64_t> common_codes;
    common_codes.reserve(ALL_CASES_NUM_);
    for (uint64_t head = 0; head < 16; ++head) {
        for (auto range : AllCases::instance().fetch()[head]) {
            common_codes.emplace_back(head << 32 | range);
        }
    }
    return common_codes;
}
