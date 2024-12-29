#include <algorithm>
#include <iostream>
#include <format>
#include <map>
#include <unordered_set>
#include <ranges/ranges.h>

#include "group/group.h"
#include "mover/mover.h"
#include "analyse/analyse.h"
#include "raw_code/raw_code.h"
#include "fast_cal/fast_cal.h"
#include "all_cases/all_cases.h"
#include "short_code/short_code.h"
#include "common_code/common_code.h"

#include <parallel_hashmap/phmap.h>

using klotski::Analyse;

using klotski::mover::S2Mover;
using klotski::mover::MaskMover;
using klotski::fast_cal::FastCal;

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::codec::RawCode;
using klotski::codec::ShortCode;
using klotski::codec::CommonCode;
using klotski::group::GroupUnion;

using klotski::group::Group;
using klotski::group::GroupCases;
using klotski::group::GroupUnion;

using klotski::group::BLOCK_NUM;
using klotski::group::TYPE_ID_LIMIT;
using klotski::cases::ALL_CASES_NUM_;
using klotski::codec::SHORT_CODE_LIMIT;

using klotski::group::PATTERN_DATA;
using klotski::group::PATTERN_OFFSET;

int main() {
    // const auto start = clock();

    // auto ret = klotski::cases::Group::extend(RawCode::from_common_code(0xDAAFE0C00).value());
    // std::cout << ret.size() << std::endl;

    const auto start = std::chrono::system_clock::now();

    // constexpr std::array s2_type_a_up = {"1a9bf0c", "1a99c3", "1abaf0c", "1aaef0c", "1aaac3", "9aaac3"};
    // constexpr std::array s2_type_a_down = {"dc3be68", "dc399a", "dc3bae8", "dc3aee8", "dc3aaa", "5c3aaa"};
    // constexpr std::array s2_type_a_left = {"1a9bfc", "1a9bd"};
    // constexpr std::array s2_type_a_right = {"1a9bc3c", "1a9bc1"};

    // const auto code = CommonCode::from_string(s2_type_a_up[5]).value().to_raw_code();
    // const auto code = CommonCode::from_string(s2_type_a_down[5]).value().to_raw_code();
    // const auto code = CommonCode::from_string(s2_type_a_left[1]).value().to_raw_code();
    // const auto code = CommonCode::from_string(s2_type_a_right[1]).value().to_raw_code();

    // constexpr std::array s2_type_b_up = {"5fef84d", "5fea134"};
    // constexpr std::array s2_type_b_down = {"9346fbf", "9346afc"};
    // constexpr std::array s2_type_b_left = {"6bfaf1c", "6bfae1", "6bfaf34", "6bfa47", "6bfa4d", "dbf993"};
    // constexpr std::array s2_type_b_right = {"4fea3cd", "4fea2c4", "4fea3c7", "4fea134", "4fea11c", "dfe660c"};

    // const auto code = CommonCode::from_string(s2_type_b_up[1]).value().to_raw_code();
    // const auto code = CommonCode::from_string(s2_type_b_down[1]).value().to_raw_code();
    // const auto code = CommonCode::from_string(s2_type_b_left[5]).value().to_raw_code();
    // const auto code = CommonCode::from_string(s2_type_b_right[5]).value().to_raw_code();

    // constexpr std::array s1_up = {"5f2f87d", "5f2a1f4"};
    // constexpr std::array s1_down = {"9f46fbc", "9f46af"};
    // constexpr std::array s1_left = {"1a9bcf", "1a9bc4"};
    // constexpr std::array s1_right = {"1a9bcf", "1a9bc4"};
    // const auto code = CommonCode::from_string(s1_up[1]).value().to_raw_code();
    // const auto code = CommonCode::from_string(s1_down[1]).value().to_raw_code();
    // const auto code = CommonCode::from_string(s1_left[1]).value().to_raw_code();
    // const auto code = CommonCode::from_string(s1_right[1]).value().to_raw_code();

    auto code = CommonCode::unsafe_create(0x1A9BF0C00).to_raw_code();
    S2Mover mover([](uint64_t code) {
        std::cout << RawCode::unsafe_create(code) << std::endl;
        if (!RawCode::check(code)) {
            std::cout << "error" << std::endl;
            std::abort();
        }
    });
    mover.next_cases(code.unwrap());

    // TODO: maybe we can support `std::format`

    // const auto code = CommonCode::unsafe_create(0x1A9BF0C00).to_raw_code();
    // const auto solve_1 = CommonCode::unsafe_create(0xDAAF4CC00).to_raw_code();
    // const auto solve_2 = CommonCode::unsafe_create(0xDAA7F3000).to_raw_code();

    // Analyse analyse {code};
    // analyse.build();
    // const auto backtrack = analyse.backtrack({solve_1, solve_2});
    // std::cout << backtrack.size() << std::endl;
    // std::cout << backtrack[0].size() << ", " << backtrack[81].size() << std::endl;

    // const auto code = CommonCode::unsafe_create(0x1A9BF0C00).to_raw_code();
    // const auto code = CommonCode::unsafe_create(0x4FEA13400).to_raw_code();
    // FastCal fc {code};

    // std::cout << fc.solve().value() << std::endl;

    // for (const auto x : fc.solve_multi()) {
    //     std::cout << x.to_common_code() << std::endl;
    // }
    //
    // for (const auto x : fc.furthest()) {
    //     std::cout << x.to_common_code() << std::endl;
    // }
    //
    // fc.build_all();
    // for (const auto &layer : fc.exports()) {
    //     std::cout << layer.size() << std::endl;
    // }
    // std::cout << "layer num: " << fc.exports().size() << std::endl;

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
