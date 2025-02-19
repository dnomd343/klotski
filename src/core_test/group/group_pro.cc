#include <gtest/gtest.h>

#include <unordered_set>

#include "helper/group.h"
#include "group/group.h"

#include "helper/parallel.h"

using klotski::group::Group;
using klotski::group::GroupUnion;

TEST(Group, cases) {
    GROUP_UNION_PARALLEL({
        for (auto group : group_union.groups()) {
            const auto &cases = helper::group_cases(group.type_id(), group.pattern_id(), (uint32_t)group.toward());
            EXPECT_EQ(group.size(), cases.size());
            EXPECT_EQ(group.cases().codes(), cases);

            EXPECT_EQ((int)group.mirror_type(), helper::pattern_mirror_type(group.type_id(), group.pattern_id()));

            auto g1 = Group::from_common_code(cases.front());
            EXPECT_EQ(g1.type_id(), group.type_id());
            EXPECT_EQ(g1.pattern_id(), group.pattern_id());
            EXPECT_EQ(g1.toward(), group.toward());

            auto g2 = Group::from_common_code(cases.back());
            EXPECT_EQ(g2.type_id(), group.type_id());
            EXPECT_EQ(g2.pattern_id(), group.pattern_id());
            EXPECT_EQ(g2.toward(), group.toward());
        }
    });
}

TEST(Group, v_mirror) {
    GROUP_UNION_PARALLEL({
        for (auto group : group_union.groups()) {
            auto g = group.to_vertical_mirror();
            EXPECT_EQ(group.type_id(), g.type_id());
            EXPECT_EQ(group.pattern_id(), g.pattern_id());
            if (group.toward() == g.toward()) {
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

TEST(Group, h_mirror) {
    GROUP_UNION_PARALLEL({
        for (auto group : group_union.groups()) {
            auto g = group.to_horizontal_mirror();
            EXPECT_EQ(group.type_id(), g.type_id());
            EXPECT_EQ(group.pattern_id(), g.pattern_id());
            if (group.toward() == g.toward()) {
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
