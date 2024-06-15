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

using klotski::codec::SHORT_CODE_LIMIT;

int main() {
    // const auto start = clock();

    // auto ret = klotski::cases::Group::extend(RawCode::from_common_code(0xDAAFE0C00).value());
    // std::cout << ret.size() << std::endl;

    const auto start = std::chrono::system_clock::now();

    // auto kk = AllCases::instance().fetch()[1];
    // for (auto &x : kk) {
    //     x >>= 20;
    // }
    // std::cout << kk.size() << std::endl;

    // for (int i = 0; i < 4096; ++i) {
    //     auto pp = std::ranges::lower_bound(kk.begin(), kk.end(), i);
    //     std::cout << pp - kk.begin() << std::endl;
    // }

    // std::unordered_set<uint32_t> tmp;
    // for (auto x : kk) {
    //     tmp.emplace(x);
    // }
    // std::cout << tmp.size() << std::endl;

    // ShortCode::speed_up(true);
    // ShortCode::speed_up(false);

    // auto show_with_index = [](uint32_t index) {
    //     uint64_t head = 1;
    //     uint32_t range = BasicRanges::instance().fetch()[index];
    //     std::cout << CommonCode::unsafe_create(head << 32 | range) << std::endl;
    // };
    //
    // for (int offset = -29; offset < 42; ++offset) {
    //     std::cout << "offset = " << offset << " | ";
    //     show_with_index(4273474 + offset);
    // }

    auto short_code = ShortCode::unsafe_create(4091296);
    std::cout << short_code.to_common_code() << std::endl;

    auto common_code = CommonCode::unsafe_create(0x1A9BF0C00);
    std::cout << common_code.to_short_code().unwrap() << std::endl;

    // auto kk = BasicRanges::instance().fetch();
    // for (auto &x : kk) {
    //     x >>= 20;
    // }
    // std::cout << kk.size() << std::endl;
    // for (int i = 0; i < 4096; ++i) {
    //     auto pp = std::ranges::lower_bound(kk.begin(), kk.end(), i);
    //     std::cout << pp - kk.begin() << std::endl;
    // }

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
