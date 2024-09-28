#include <algorithm>
#include <thread>
#include <iostream>
#include <format>
#include <map>
#include <unordered_set>
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

using klotski::cases::Group;
using klotski::cases::GroupCases;
using klotski::cases::GroupUnion;

using klotski::cases::TYPE_ID_LIMIT;
using klotski::codec::SHORT_CODE_LIMIT;

int main() {
    // const auto start = clock();

    // auto ret = klotski::cases::Group::extend(RawCode::from_common_code(0xDAAFE0C00).value());
    // std::cout << ret.size() << std::endl;

    const auto start = std::chrono::system_clock::now();

//    const auto common_code = CommonCode::unsafe_create(0x1A9BF0C00);
//    const auto group = Group::from_common_code(common_code);

//    GroupCases::build();
//
//    std::cout << GroupCases::fast_parse({169, 1, 7472}) << std::endl;
//    std::cout << GroupCases::fast_parse({164, 0, 30833}) << std::endl;
//
//    std::cout << GroupCases::fast_obtain(CommonCode::unsafe_create(0x1A9BF0C00)) << std::endl;
//    std::cout << GroupCases::fast_obtain(CommonCode::unsafe_create(0x4FEA13400)) << std::endl;

    auto group = Group::unsafe_create(123, 1446);

    std::cout << group.to_horizontal_mirror().group_id() << std::endl;
    std::cout << group.to_vertical_mirror().group_id() << std::endl;

    std::cerr << std::chrono::system_clock::now() - start << std::endl;

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
