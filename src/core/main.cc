#include <algorithm>
#include <thread>
#include <iostream>
#include <format>

#include "core/core.h"
#include "group/group.h"
#include "raw_code/raw_code.h"
#include "fast_cal/fast_cal.h"
#include "all_cases/all_cases.h"
#include "short_code/short_code.h"
#include "common_code/common_code.h"

using klotski::core::Core;

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::codec::RawCode;
using klotski::codec::ShortCode;
using klotski::codec::CommonCode;
using klotski::cases::GroupUnion;

using klotski::codec::SHORT_CODE_LIMIT;

int main() {
    const auto start = clock();

    klotski::cases::spawn_ranges(2, 1, 4, 4);

    // std::vector<int> series {1, 2, 3, 4};

    // do { // full permutation traversal
    //
    //     for (auto s : series) {
    //         std::cout << s << " ";
    //     }
    //     std::cout << std::endl;
    //
    // } while (std::next_permutation(series.begin(), series.end()));

    // std::array a{'a', 'b', 'c'};
    // do {
    //     for (auto x : a) {
    //         std::cout << x;
    //     }
    //     std::cout << std::endl;
    // }
    // while (std::ranges::next_permutation(a).found);

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

    std::cerr << ((clock() - start) * 1000 / CLOCKS_PER_SEC) << "ms" << std::endl;

    return 0;
}
