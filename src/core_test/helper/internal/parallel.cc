#include "helper/parallel.h"

#include <BS_thread_pool.hpp>

#include "group/group.h"
#include "all_cases/all_cases.h"

using klotski::cases::AllCases;
using klotski::cases::TYPE_ID_LIMIT;
using klotski::cases::ALL_CASES_NUM_;

void helper::group_parallel(std::function<void(Group)> &&func) {
    // TODO: spawn all Groups
}

void helper::type_id_parallel(std::function<void(uint32_t type_id)> &&func) {

    BS::thread_pool pool;

    // TODO: using `detach_sequence`
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {

        pool.detach_task([type_id, &func]() {
            func(type_id);
        });

    }

    pool.wait();

}

void helper::group_union_parallel(std::function<void(GroupUnion)> &&func) {
    type_id_parallel([&func](uint32_t type_id) {
        func(GroupUnion::unsafe_create(type_id));
    });
}

void helper::common_code_parallel(std::function<void(std::span<CommonCode>)> &&func) {

    static auto codes = AllCases::instance().fetch().codes();

    BS::thread_pool pool;

    // TODO: enhance performance

    pool.detach_blocks((uint64_t)0, codes.size(), [&func](auto start, auto end) {

        func(std::span<CommonCode> {codes.data() + start, end - start});

    }, 16);

    pool.wait();

}

void helper::raw_code_parallel(std::function<void(std::span<RawCode>)> &&func) {

    auto common_codes = AllCases::instance().fetch().codes();
    static auto codes = std::vector<RawCode> {common_codes.begin(), common_codes.end()};

    BS::thread_pool pool;
    pool.detach_blocks((uint64_t)0, codes.size(), [&func](auto start, auto end) {

        func(std::span<RawCode> {codes.data() + start, end - start});

    }, 16);

    pool.wait();

}

void helper::short_code_parallel(std::function<void(std::span<ShortCode>)> &&func) {
    static auto codes = []() {
        std::vector<uint32_t> v (klotski::codec::SHORT_CODE_LIMIT);
        std::iota(v.begin(), v.end(), 0);
        return v;
    }();

    BS::thread_pool pool;
    pool.detach_blocks((uint64_t)0, codes.size(), [&func](auto start, auto end) {

        auto span = std::span<uint32_t> {codes.data() + start, end - start};
        func(std::bit_cast<std::span<ShortCode>>(span));

    }, 16);

    pool.wait();
}
