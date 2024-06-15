#include <algorithm>
#include <thread>
#include <iostream>
#include <format>
#include <ranges/ranges.h>

#include "core/core.h"
#include "group/group.h"
#include "raw_code/raw_code.h"
#include "fast_cal/fast_cal.h"
#include "all_cases/all_cases.h"
#include "short_code/short_code.h"
#include "common_code/common_code.h"

#include "../../third_party/thread-pool/include/BS_thread_pool.hpp"

using klotski::core::Core;

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::codec::RawCode;
using klotski::codec::ShortCode;
using klotski::codec::CommonCode;
using klotski::cases::GroupUnion;

using klotski::codec::SHORT_CODE_LIMIT;

int main() {
    // const auto start = clock();

    // auto ret = klotski::cases::Group::extend(RawCode::from_common_code(0xDAAFE0C00).value());
    // std::cout << ret.size() << std::endl;

    const auto start = std::chrono::system_clock::now();

    // AllCases::instance().build();

    for (int type_id = 0; type_id < klotski::cases::TYPE_ID_LIMIT; ++type_id) {
        auto ret = GroupUnion::unsafe_create(type_id).cases();
        // std::cout << ret[4].size() << std::endl;
    }

    // auto group_union = GroupUnion::unsafe_create(169);
    // for (auto group : group_union.groups()) {
    //     std::cout << group.type_id_ << ", " << group.group_id_ << std::endl;
    // }

    // constexpr auto gu = GroupUnion::unsafe_create(169);
    // constexpr auto gu_ = GroupUnion::create(169).value();
    // // constexpr auto gu_ = GroupUnion::create(1169).value();
    // constexpr auto k1 = gu.unwrap();
    // constexpr auto k2 = gu.size();
    // constexpr auto k3 = gu.group_num();
    // constexpr auto k4 = gu.max_group_size();

    // auto kk = GroupUnion::unsafe_create(169);
    // auto pp = kk.group(0).value();
    // std::cout << pp.type_id() << ", " << pp.group_id() << std::endl;

    // for (auto group : kk.groups()) {
    //     std::cout << group.type_id() << ", " << group.group_id() << std::endl;
    // }


    auto gu = GroupUnion::unsafe_create(169);
    // auto cases = gu.group(0).value().cases();

    // for (auto &kk : cases) {
    //     std::cout << kk.size() << std::endl;
    // }

    klotski::cases::RangesUnion cases;
    for (auto group : gu.groups()) {
        cases += group.cases();
    }
    for (auto &kk : cases) {
        std::ranges::sort(kk.begin(), kk.end());
    }
    std::cout << (cases == gu.cases()) << std::endl;

    std::cerr << std::chrono::system_clock::now() - start << std::endl;

    // auto raw_code = RawCode::from_common_code(0x1A9BF0C00)->unwrap();
    // auto ret = klotski::cases::group_extend_from_seed(raw_code);
    //
    // std::cout << ret.size() << std::endl;

    // 1 A9BF0C00 -> 10 10 10 01 10 11 11 11 00 00 11 000000
    // auto ret = klotski::cases::spawn_ranges(2, 1, 4, 4);
    // std::cout << ret.size() << std::endl;

    // auto kk = GroupUnion::create(123).value();
    // std::cout << kk.size() << std::endl;
    // std::cout << kk.group_num() << std::endl;
    // std::cout << kk.max_group_size() << std::endl;

    // auto core = Core([](const uint64_t code, uint64_t) {
    //     std::cout << RawCode::unsafe_create(code);
    //     std::cout << std::endl;
    // });
    // core.next_cases(RawCode::from_common_code(0x1A9BF0C00).value().unwrap(), 0x0);

    // auto cal = FastCal(RawCode::from_common_code(0x1A9BF0C00).value());
    // auto cal = FastCal(RawCode::from_common_code("25EEF04").value());

    // auto ret = cal.solve();
    //
    // for (auto kk : cal.backtrack(ret)) {
    //     std::cout << kk.to_common_code() << "," << kk.to_common_code().to_short_code() << std::endl;
    // }

    // for (const auto solve : cal.solve_multi()) {
    //     for (const auto raw_code : cal.backtrack(solve)) {
    //         const auto common_code = raw_code.to_common_code();
    //         std::cout << common_code << "/" << common_code.to_short_code() << std::endl;
    //     }
    //     std::cout << "----" << std::endl;
    // }

    // std::cerr << ((clock() - start) * 1000 / CLOCKS_PER_SEC) << "ms" << std::endl;

    return 0;
}
