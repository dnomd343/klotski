#include <BS_thread_pool.hpp>

#include "codec.h"

using klotski::cases::AllCases;
using klotski::cases::ALL_CASES_NUM_;

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

static std::vector<RawCode> convert(const std::vector<CommonCode> &codes) {
    std::vector<RawCode> result;
    result.reserve(29334498);
    for (auto code : codes) {
        result.emplace_back(RawCode::from_common_code(code));
    }
    return result;
}

void raw_code_parallel(std::function<void(std::span<RawCode>)> &&func) {

    static auto codes = convert(AllCases::instance().fetch().codes());

    BS::thread_pool pool;
    pool.detach_blocks((uint64_t)0, codes.size(), [func = std::move(func)](auto start, auto end) {

        func(std::span<RawCode> {codes.data() + start, end - start});

    }, 16);

    pool.wait();

}

void short_code_parallel(std::function<void(std::span<ShortCode>)> &&func) {
    static auto codes = []() {
        std::vector<uint32_t> v (klotski::codec::SHORT_CODE_LIMIT);
        std::iota(v.begin(), v.end(), 0);
        return v;
    }();

    BS::thread_pool pool;
    pool.detach_blocks((uint64_t)0, codes.size(), [func = std::move(func)](auto start, auto end) {

        auto span = std::span<uint32_t> {codes.data() + start, end - start};
        func(std::bit_cast<std::span<ShortCode>>(span));

    }, 16);

    pool.wait();
}
