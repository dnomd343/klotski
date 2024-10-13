#include <gtest/gtest.h>

#include <unordered_set>

#include "helper/group.h"
#include "group/group.h"

#include "helper/parallel.h"

using klotski::cases::GroupPro;
using klotski::cases::GroupUnion;

TEST(GroupPro, demo) {

    std::cout << helper::group_union_num() << std::endl;

    std::cout << helper::group_union_pattern_num(169) << std::endl;
    std::cout << GroupUnion::unsafe_create(169).pattern_num() << std::endl;

    std::cout << helper::group_union_group_num(169) << std::endl;
    std::cout << GroupUnion::unsafe_create(169).group_num() << std::endl;

    std::cout << (int)helper::pattern_mirror_type(169, 0) << std::endl;
    std::cout << (int)GroupPro::unsafe_create(169, 0, 0).mirror_type() << std::endl;

    std::cout << std::format("{}", helper::pattern_toward_list(169, 0)) << std::endl;
    std::cout << (int)GroupUnion::unsafe_create(169).groups_pro()[0].mirror_toward() << std::endl;
    std::cout << (int)GroupUnion::unsafe_create(169).groups_pro()[1].mirror_toward() << std::endl;

    auto group_1 = GroupUnion::unsafe_create(169).groups_pro()[0];
    EXPECT_EQ(group_1.cases().codes(), helper::group_cases(169, 0, group_1.mirror_toward()));
    auto group_2 = GroupUnion::unsafe_create(169).groups_pro()[1];
    EXPECT_EQ(group_2.cases().codes(), helper::group_cases(169, 0, group_2.mirror_toward()));

}

TEST(GroupPro, cases) {
    GROUP_UNION_PARALLEL({
        for (auto group : group_union.groups_pro()) {
            const auto &cases = helper::group_cases(group.type_id(), group.pattern_id(), group.mirror_toward());
            EXPECT_EQ(group.size(), cases.size());
            EXPECT_EQ(group.cases().codes(), cases);

            EXPECT_EQ((int)group.mirror_type(), helper::pattern_mirror_type(group.type_id(), group.pattern_id()));

            auto g1 = GroupPro::from_common_code(cases.front());
            EXPECT_EQ(g1.type_id(), group.type_id());
            EXPECT_EQ(g1.pattern_id(), group.pattern_id());
            EXPECT_EQ(g1.mirror_toward(), group.mirror_toward());

            auto g2 = GroupPro::from_common_code(cases.back());
            EXPECT_EQ(g2.type_id(), group.type_id());
            EXPECT_EQ(g2.pattern_id(), group.pattern_id());
            EXPECT_EQ(g2.mirror_toward(), group.mirror_toward());
        }
    });
}

TEST(GroupPro, v_mirror) {
    GROUP_UNION_PARALLEL({
        for (auto group : group_union.groups_pro()) {
            auto g = group.to_vertical_mirror();
            EXPECT_EQ(group.type_id(), g.type_id());
            EXPECT_EQ(group.pattern_id(), g.pattern_id());
            if (group.mirror_toward() == g.mirror_toward()) {
                EXPECT_TRUE(group.is_vertical_mirror());

                std::unordered_set<uint64_t> cases;
                for (const auto code : group.cases().codes()) {
                    cases.emplace(code);
                }
                for (const auto code : cases) {
                    auto mirror = CommonCode::unsafe_create(code).to_vertical_mirror();
                    EXPECT_TRUE(cases.contains(mirror.unwrap()));
                }
            } else {
                EXPECT_FALSE(group.is_vertical_mirror());

                std::unordered_set<uint64_t> cases_1;
                std::unordered_set<uint64_t> cases_2;
                for (const auto code : group.cases().codes()) {
                    cases_1.emplace(code);
                }
                for (const auto code : g.cases().codes()) {
                    cases_2.emplace(code);
                }
                EXPECT_EQ(cases_1.size(), cases_2.size());

                for (const auto code : cases_1) {
                    auto mirror = CommonCode::unsafe_create(code).to_vertical_mirror();
                    EXPECT_FALSE(cases_1.contains(mirror.unwrap()));
                    EXPECT_TRUE(cases_2.contains(mirror.unwrap()));
                }
            }
        }
    });
}

TEST(GroupPro, h_mirror) {
    GROUP_UNION_PARALLEL({
        for (auto group : group_union.groups_pro()) {
            auto g = group.to_horizontal_mirror();
            EXPECT_EQ(group.type_id(), g.type_id());
            EXPECT_EQ(group.pattern_id(), g.pattern_id());
            if (group.mirror_toward() == g.mirror_toward()) {
                EXPECT_TRUE(group.is_horizontal_mirror());

                std::unordered_set<uint64_t> cases;
                for (const auto code : group.cases().codes()) {
                    cases.emplace(code);
                }
                for (const auto code : cases) {
                    auto mirror = CommonCode::unsafe_create(code).to_horizontal_mirror();
                    EXPECT_TRUE(cases.contains(mirror.unwrap()));
                }
            } else {
                EXPECT_FALSE(group.is_horizontal_mirror());

                std::unordered_set<uint64_t> cases_1;
                std::unordered_set<uint64_t> cases_2;
                for (const auto code : group.cases().codes()) {
                    cases_1.emplace(code);
                }
                for (const auto code : g.cases().codes()) {
                    cases_2.emplace(code);
                }
                EXPECT_EQ(cases_1.size(), cases_2.size());

                for (const auto code : cases_1) {
                    auto mirror = CommonCode::unsafe_create(code).to_horizontal_mirror();
                    EXPECT_FALSE(cases_1.contains(mirror.unwrap()));
                    EXPECT_TRUE(cases_2.contains(mirror.unwrap()));
                }
            }
        }
    });
}
