#include <algorithm>
#include <thread>
#include <iostream>
#include <format>
#include <map>
#include <unordered_set>
#include <ranges/ranges.h>

#include "group/group.h"
#include "mover/mover.h"
#include "raw_code/raw_code.h"
#include "fast_cal/fast_cal.h"
#include "all_cases/all_cases.h"
#include "short_code/short_code.h"
#include "common_code/common_code.h"

#include "../../third_party/thread-pool/include/BS_thread_pool.hpp"

using klotski::mover::MaskMover;

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
using klotski::cases::ALL_CASES_NUM_;
using klotski::codec::SHORT_CODE_LIMIT;

using klotski::cases::PATTERN_DATA;
using klotski::cases::PATTERN_OFFSET;
using klotski::cases::Group_load_factor;

int main() {
    // const auto start = clock();

    // auto ret = klotski::cases::Group::extend(RawCode::from_common_code(0xDAAFE0C00).value());
    // std::cout << ret.size() << std::endl;

    const auto start = std::chrono::system_clock::now();

    auto code = CommonCode::unsafe_create(0x1A9BF0C00).to_raw_code();
    for (int i = 0; i < 100; ++i) {
//        FastCal fc {code};
//        fc.solve();
//        klotski::cases::Group_extend(code);
        FastCal_demo(code);
        break;
    }

//    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
//        auto group_union = GroupUnion::unsafe_create(type_id);
//        for (uint32_t pattern_id = 0; pattern_id < group_union.pattern_num(); ++pattern_id) {
//            std::cout << std::format("[{}, {}]\n", type_id, pattern_id);
//            auto seed = CommonCode::unsafe_create(PATTERN_DATA[PATTERN_OFFSET[type_id] + pattern_id] >> 23);
//
//            double coff = 1.0;
//            double last_val = -1;
//            while (true) {
//                auto val = Group_load_factor(seed.to_raw_code(), coff);
//                if (int(val * 1000) != int(last_val * 1000)) {
//                    std::cout << std::format("{:.2f}, {:.6f}\n", coff, val);
//                    last_val = val;
//                }
//                if (coff >= 2.0) {
//                    break;
//                }
//                coff += 0.01;
//            }
//            std::cout << std::endl;
//        }
//
//    }

//    std::cout << Group_load_factor(code, 0.5) << std::endl;
//    std::cout << Group_load_factor(code, 0.8) << std::endl;
//    std::cout << Group_load_factor(code, 1.0) << std::endl;
//    std::cout << Group_load_factor(code, 1.2) << std::endl;
//    std::cout << Group_load_factor(code, 1.5) << std::endl;
//    std::cout << Group_load_factor(code, 2.0) << std::endl;
//    std::cout << Group_load_factor(code, 3.0) << std::endl;
//    std::cout << Group_load_factor(code, 5.0) << std::endl;

//    for (int i = 0; i < 10000000; ++i) {
//        MaskMover mover([](uint64_t code, uint64_t mask) {
//            volatile auto tmp_1 = code;
//            volatile auto tmp_2 = mask;
//        });
//        mover.next_cases(0x1A9BF0C00, 0);
//    }

//    ShortCode::speed_up(true);
//
//    std::unordered_set<RawCode> data_r;
//    std::unordered_set<ShortCode> data_s;
//    std::unordered_set<CommonCode> data_c;
//    data_r.reserve(ALL_CASES_NUM_);
//    data_s.reserve(ALL_CASES_NUM_);
//    data_c.reserve(ALL_CASES_NUM_);
//    for (auto code : AllCases::instance().fetch().codes()) {
//        data_c.emplace(code);
//        data_r.emplace(code.to_raw_code());
//        data_s.emplace(code.to_short_code());
//    }
//    std::cout << data_r.size() << std::endl;
//    std::cout << data_s.size() << std::endl;
//    std::cout << data_c.size() << std::endl;

//    auto group_union = GroupUnion::unsafe_create(169);
//    std::cout << group_union << std::endl;

//    auto group = Group::from_common_code(CommonCode::unsafe_create(0x1A9BF0C00));
//    std::cout << group << std::endl;
//    std::cout << group.type_id() << std::endl;
//    std::cout << group.pattern_id() << std::endl;
//    std::cout << (int)group.toward() << std::endl;
//    std::cout << group.toward_char() << std::endl;
//    std::cout << group.to_string() << std::endl;

//    auto info_1 = GroupCases::obtain_info(CommonCode::unsafe_create(0x1A9BF0C00));
//    std::cout << info_1 << std::endl;
//    auto code_1 = GroupCases::obtain_code(info_1);
//    std::cout << code_1 << std::endl;
//
//    auto info_2 = GroupCases::obtain_info(CommonCode::unsafe_create(0x1A9BF0C00).to_short_code());
//    std::cout << info_2 << std::endl;
//    auto code_2 = GroupCases::obtain_code(info_2);
//    std::cout << code_2 << std::endl;
//
//    GroupCases::build();
//
//    auto info_3 = GroupCases::obtain_info(CommonCode::unsafe_create(0x1A9BF0C00));
//    std::cout << info_3 << std::endl;
//    auto code_3 = GroupCases::obtain_code(info_3);
//    std::cout << code_3 << std::endl;
//
//    auto info_4 = GroupCases::obtain_info(CommonCode::unsafe_create(0x1A9BF0C00).to_short_code());
//    std::cout << info_4 << std::endl;
//    auto code_4 = GroupCases::obtain_code(info_4);
//    std::cout << code_4 << std::endl;

//    const auto common_code = CommonCode::unsafe_create(0x1A9BF0C00);
//    const auto group = Group::from_common_code(common_code);

//    GroupCases::build();
//
//    std::cout << GroupCases::fast_parse({169, 1, 7472}) << std::endl;
//    std::cout << GroupCases::fast_parse({164, 0, 30833}) << std::endl;
//
//    std::cout << GroupCases::fast_obtain(CommonCode::unsafe_create(0x1A9BF0C00)) << std::endl;
//    std::cout << GroupCases::fast_obtain(CommonCode::unsafe_create(0x4FEA13400)) << std::endl;

//    std::cout << gu.pattern_num() << std::endl;
//    std::cout << gu.group_num() << std::endl;
//    std::cout << gu.groups_pro().size() << std::endl;
//
//    const auto g = gu.groups_pro()[1];
//    std::cout << std::format("{}-{}-{}\n", g.type_id(), g.pattern_id(), g.mirror_toward());
//
//    const auto codes = g.cases().codes();
//    std::cout << codes.size() << std::endl;
//    for (const auto code : codes) {
//        if (code == 0x1A9BF0C00) {
//            std::cout << "get it" << std::endl;
//        }
//    }
//
//    std::cout << g.is_vertical_mirror() << std::endl;
//    std::cout << g.is_horizontal_mirror() << std::endl;
//
//    auto g1 = g.to_vertical_mirror();
//    std::cout << std::format("{}-{}-{}\n", g1.type_id(), g1.pattern_id(), g1.mirror_toward());
//    auto g2 = g.to_horizontal_mirror();
//    std::cout << std::format("{}-{}-{}\n", g2.type_id(), g2.pattern_id(), g2.mirror_toward());

//    auto common_code = CommonCode::unsafe_create(0x1A9BF0C00);
//    auto common_code = CommonCode::unsafe_create(0x4FEA13400);
//    auto kk = GroupPro::from_common_code(common_code);
//    std::cout << std::format("{}-{}-{}\n", kk.type_id(), kk.pattern_id(), kk.mirror_toward());

//    auto gp = GroupPro::unsafe_create(169, 0, 0);
//    std::cout << gp.size() << std::endl;
//    std::cout << (int)gp.mirror_type() << std::endl;

#if defined(__clang__)
    std::cerr << (std::chrono::system_clock::now() - start).count() / 1000 << "ms" << std::endl;
#elif defined(__GNUC__)
    std::cerr << (std::chrono::system_clock::now() - start).count() / 1000000 << "ms" << std::endl;
#endif

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
