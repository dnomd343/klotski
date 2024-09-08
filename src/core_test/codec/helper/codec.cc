#include <BS_thread_pool.hpp>

#include "codec.h"

void head_parallel(std::function<void(uint64_t head)> &&func) {
    constexpr auto heads = std::to_array({
        0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14
    });
    BS::thread_pool pool;
    for (auto head : heads) {
        pool.detach_task([head, &func] {
            func(head);
        });
    }
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

void common_code_parallel(std::function<void(std::span<CommonCode>)> &&func) {

    static auto codes = AllCases::instance().fetch().codes();

    BS::thread_pool pool;

    // TODO: enhance performance

    pool.detach_blocks((uint64_t)0, codes.size(), [func = std::move(func)](auto start, auto end) {

        func(std::span<CommonCode> {codes.data() + start, end - start});

    }, 16);

    pool.wait();

}
