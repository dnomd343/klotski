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

using klotski::codec::SHORT_CODE_LIMIT;

int main() {
    // const auto start = clock();

    // auto ret = klotski::cases::Group::extend(RawCode::from_common_code(0xDAAFE0C00).value());
    // std::cout << ret.size() << std::endl;

    const auto start = std::chrono::system_clock::now();

    // auto ret = CommonCode::is_mirror(0x1A9BF0C00);
    // auto ret = CommonCode::is_mirror(0x4FEA13400);
    // auto ret = CommonCode::is_mirror(0x100AA0300);
    // auto ret = CommonCode::is_mirror(0x5000A0000);
    // std::cout << "ret = " << ret << std::endl;

    // auto raw_code = CommonCode::unsafe_create(0x1A9BF0C00).to_raw_code();
    // std::cout << raw_code.is_horizontal_mirror() << std::endl;

    // for (auto common_code : AllCases::instance().fetch().codes()) {
    //     auto raw_code = common_code.to_raw_code();
    //
    //     bool r1 = raw_code.is_horizontal_mirror();
    //     bool r2 = CommonCode::is_mirror(common_code.unwrap());
    //
    //     if (r1 != r2) {
    //         std::cout << "found invalid" << std::endl;
    //         std::cout << common_code << std::endl;
    //         break;
    //     }
    // }

    // GroupCases::from_info_t({169, 1, 7472});

    // GroupCases::to_info_t(CommonCode::unsafe_create(0x1A9BF0C00).to_short_code());

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
