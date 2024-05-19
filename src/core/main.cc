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

    // std::cout << klotski::cases::common_code_to_type_id(0x1A9BF0C00) << std::endl;

    // uint32_t demo = 0b101001110110;
    //                 010110001001
    //                 01001000100

    // 00 -> 1 | 0 -> 0
    // 01 -> 1 | 1 -> 1
    // 10 -> 0 | 0 -> 0
    // 11 -> 0 | 1 -> 0

    // uint32_t ret = ((~demo >> 1) & demo) & 0x55555555;
    //
    // std::cout << ret << std::endl;
    // std::cout << std::popcount(ret) << std::endl;

    uint32_t range = 0xA9BF0C00; // n_01 = 1 / n_10 = 4 / n_11 = 4

    // 10 10 10 01 10 11 11 11 00 00 11 00 00 00 00 00
    //  0  0  0  1  0  0  0  0  1  1  0  1  1  1  1  1
    //  0  0  0  1  0  1  1  1  0  0  1  0  0  0  0  0
    //  0  0  0  1  0  0  0  0  0  0  0  0  0  0  0  0

    uint32_t k_01 = (~range >> 1) & range & 0x55555555;
    uint32_t k_10 = (range >> 1) & ~range & 0x55555555;
    uint32_t k_11 = (range >> 1) & range & 0x55555555;

    // std::cout << k_01 << std::endl;
    std::cout << std::popcount(k_01) << std::endl;
    std::cout << std::popcount(k_10) << std::endl;
    std::cout << std::popcount(k_11) << std::endl;

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
