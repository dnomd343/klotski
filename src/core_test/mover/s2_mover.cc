#include <gtest/gtest.h>

#include "group/group.h"
#include "mover/mover.h"
#include "raw_code/raw_code.h"
#include "all_cases/all_cases.h"

using klotski::codec::RawCode;
using klotski::cases::AllCases;

using klotski::mover::S2Mover;
using klotski::mover::MaskMover;

using klotski::group::BLOCK_NUM;
using klotski::group::GroupUnion;

TEST(S2Mover, demo) {
    auto common_mover = [](RawCode src) {
        std::vector<uint64_t> results {};
        auto mover = MaskMover([&results](RawCode code, uint64_t) {
            results.emplace_back(code.unwrap());
        });
        mover.next_cases(src, 0);
        std::ranges::sort(results.begin(), results.end());
        return results;
    };

    auto s2_mover = [](RawCode src) {
        std::vector<uint64_t> results {};
        auto mover = S2Mover([&results](RawCode code) {
            // std::cout << RawCode::unsafe_create(code) << std::endl;
            EXPECT_TRUE(RawCode::check(code.unwrap()));
            results.emplace_back(code);
        });
        mover.next_cases(src);
        std::ranges::sort(results.begin(), results.end());
        return results;
    };

    for (auto code : AllCases::instance().fetch().codes()) {
        // TODO: only check every type_id -> loop GroupUnion
        auto type_id = GroupUnion::from_common_code(code).unwrap();
        auto [n, n_2x1, n_1x1] = BLOCK_NUM[type_id];
        if (n * 2 + n_1x1 != 14) {
            continue; // not s2 cases
        }

        auto raw_code = code.to_raw_code();
        EXPECT_EQ(common_mover(raw_code), s2_mover(raw_code));
    }
}
