#include <gtest/gtest.h>

#include <ranges>
#include <cstdint>
#include <algorithm>
#include <unordered_set>

#include "helper/hash.h"
#include "helper/group.h"
#include "helper/expect.h"
#include "helper/parallel.h"
#include "helper/block_num.h"

#include "group/group.h"
#include "common_code/common_code.h"

using klotski::group::Group;
using klotski::codec::ShortCode;
using klotski::group::GroupUnion;

using klotski::group::BLOCK_NUM;
using klotski::group::GROUP_NUM;
using klotski::group::PATTERN_NUM;
using klotski::group::GROUP_OFFSET;
using klotski::group::ALL_GROUP_NUM;
using klotski::group::TYPE_ID_LIMIT;
using klotski::group::MAX_GROUP_SIZE;
using klotski::group::PATTERN_OFFSET;
using klotski::group::ALL_PATTERN_NUM;
using klotski::group::GROUP_UNION_SIZE;
using klotski::group::GROUP_UNION_CASES_NUM;

static_assert(helper::is_hashable_v<GroupUnion>);
static_assert(helper::is_plain_class_v<GroupUnion>);
static_assert(!std::is_default_constructible_v<GroupUnion>);

#define EXPECT_SUM_EQ(ARR, SUM) \
    EXPECT_EQ(std::accumulate(ARR.begin(), ARR.end(), 0), SUM)

#define EXPECT_OFFSET(ARR, OFFSET) \
    EXPECT_EQ(ARR.size(), OFFSET.size()); \
    EXPECT_FALSE(ARR.empty()) \
    // TODO: offset test

#define TYPE_ID_LOOP(impl) \
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) { \
        const auto group_union = GroupUnion::unsafe_create(type_id); \
        EXPECT_EQ(group_union.unwrap(), type_id); \
        {impl} \
    }\

TEST(GroupUnion, basic) {
    TYPE_ID_LOOP({
        EXPECT_TRUE(GroupUnion::create(type_id).has_value());
        EXPECT_EQ(GroupUnion::create(type_id).value().unwrap(), type_id);
        EXPECT_EQ(GroupUnion::unsafe_create(type_id).unwrap(), type_id);
    });
    EXPECT_FALSE(GroupUnion::create(-1).has_value());
    EXPECT_FALSE(GroupUnion::create(TYPE_ID_LIMIT).has_value());

    TYPE_ID_LOOP({
        EXPECT_EQ(group_union, group_union); // test `operator==`
        EXPECT_NE(group_union, GroupUnion::unsafe_create(TYPE_ID_LIMIT)); // test `operator!=`
    })

#ifndef KLSK_NDEBUG
    TYPE_ID_LOOP({
        EXPECT_OSTREAM(group_union, std::to_string(type_id)); // test `operator<<`
    })
#endif
}

TEST(GroupUnion, type_id) {
    ShortCode::speed_up(true);
    COMMON_CODE_PARALLEL({
         const auto type_id = helper::to_type_id(code.unwrap());
         EXPECT_EQ(GroupUnion::from_common_code(code).unwrap(), type_id);
         EXPECT_EQ(GroupUnion::from_raw_code(code.to_raw_code()).unwrap(), type_id);
         EXPECT_EQ(GroupUnion::from_short_code(code.to_short_code()).unwrap(), type_id);
    });
}

TEST(GroupUnion, values) {
    GROUP_UNION_PARALLEL({
        const auto type_id = group_union.unwrap();
        EXPECT_EQ(group_union.size(), helper::group_union_cases(type_id).size());
        EXPECT_EQ(group_union.group_num(), helper::group_union_group_num(type_id));
        EXPECT_EQ(group_union.pattern_num(), helper::group_union_pattern_num(type_id));

        const auto sizes = std::views::iota(0U, group_union.pattern_num())
                         | std::views::transform([type_id](const auto pattern_id) {
                               return helper::pattern_group_size(type_id, pattern_id);
                           });
        EXPECT_EQ(group_union.max_group_size(), *std::ranges::max_element(sizes));
    });
}

TEST(GroupUnion, constant) {
    // test TYPE_ID_LIMIT
    EXPECT_EQ(TYPE_ID_LIMIT, helper::group_union_num());

    // test GROUP_NUM / GROUP_OFFSET / ALL_GROUP_NUM
    EXPECT_EQ(GROUP_NUM.size(), TYPE_ID_LIMIT);
    TYPE_ID_LOOP({
        EXPECT_EQ(GROUP_NUM[type_id], group_union.group_num());
    })
    EXPECT_OFFSET(GROUP_NUM, GROUP_OFFSET);
    EXPECT_SUM_EQ(GROUP_NUM, ALL_GROUP_NUM);

    // test PATTERN_NUM / PATTERN_OFFSET / ALL_PATTERN_NUM
    EXPECT_EQ(PATTERN_NUM.size(), TYPE_ID_LIMIT);
    TYPE_ID_LOOP({
        EXPECT_EQ(PATTERN_NUM[type_id], group_union.pattern_num());
    })
    EXPECT_OFFSET(PATTERN_NUM, PATTERN_OFFSET);
    EXPECT_SUM_EQ(PATTERN_NUM, ALL_PATTERN_NUM);

    // test MAX_GROUP_SIZE
    EXPECT_EQ(MAX_GROUP_SIZE.size(), TYPE_ID_LIMIT);
    TYPE_ID_LOOP({
        EXPECT_EQ(MAX_GROUP_SIZE[type_id], group_union.max_group_size());
    })

    // test GROUP_UNION_SIZE
    EXPECT_EQ(GROUP_UNION_SIZE.size(), TYPE_ID_LIMIT);
    TYPE_ID_LOOP({
        EXPECT_EQ(GROUP_UNION_SIZE[type_id], group_union.size());
    })

    // test BLOCK_NUM
    EXPECT_EQ(BLOCK_NUM.size(), TYPE_ID_LIMIT);
    TYPE_ID_LOOP({
        const auto num = BLOCK_NUM[type_id];
        const auto val = helper::to_block_num(type_id);
        EXPECT_EQ(std::get<0>(num), val.n_1x2 + val.n_2x1);
        EXPECT_EQ(std::get<1>(num), val.n_2x1);
        EXPECT_EQ(std::get<2>(num), val.n_1x1);
    })

    // test GROUP_UNION_CASES_NUM
    EXPECT_EQ(GROUP_UNION_CASES_NUM.size(), TYPE_ID_LIMIT);
    TYPE_ID_LOOP({
        const auto cases = group_union.cases();
        const auto num = GROUP_UNION_CASES_NUM[type_id];
        EXPECT_EQ(std::get<0>(num), cases.ranges(0x0).size());
        EXPECT_EQ(std::get<1>(num), cases.ranges(0x1).size());
        EXPECT_EQ(std::get<0>(num), cases.ranges(0x2).size());

        EXPECT_EQ(std::get<2>(num), cases.ranges(0x4).size());
        EXPECT_EQ(std::get<3>(num), cases.ranges(0x5).size());
        EXPECT_EQ(std::get<2>(num), cases.ranges(0x6).size());

        EXPECT_EQ(std::get<2>(num), cases.ranges(0x8).size());
        EXPECT_EQ(std::get<3>(num), cases.ranges(0x9).size());
        EXPECT_EQ(std::get<2>(num), cases.ranges(0xA).size());

        EXPECT_EQ(std::get<0>(num), cases.ranges(0xC).size());
        EXPECT_EQ(std::get<1>(num), cases.ranges(0xD).size());
        EXPECT_EQ(std::get<0>(num), cases.ranges(0xE).size());
    })
}

TEST(GroupUnion, cases) {
    static std::array<std::vector<CommonCode>, 203> kk {};
    GROUP_UNION_PARALLEL({
        const auto type_id = group_union.unwrap();
        const auto cases = group_union.cases().codes();
        for (auto code : cases) {
            EXPECT_EQ(type_id, helper::to_type_id(code.unwrap()));
        }
        EXPECT_EQ(cases, helper::group_union_cases(type_id));

        kk[type_id] = cases;

    });

    std::vector<CommonCode> result {};
    for (auto &codes : kk) {
        result.insert(result.end(), codes.begin(), codes.end());
    }
    std::ranges::sort(result.begin(), result.end());
    EXPECT_EQ(result, klotski::cases::AllCases::instance().fetch().codes());
}

// #define EXPECT_DIFF(ARR) \
//     std::unordered_set diff_set {ARR}; \
//     EXPECT_EQ(diff_set.size(), ARR.size())

TEST(GroupUnion, groups) {
    GROUP_UNION_PARALLEL({
        const auto type_id = group_union.unwrap();

        auto groups = group_union.groups();
        EXPECT_EQ(groups.size(), group_union.group_num());
        for (uint32_t pattern_id = 0; pattern_id < group_union.pattern_num(); ++pattern_id) {
            std::vector<uint32_t> towards;
            for (auto group : groups) {
                if (group.pattern_id() == pattern_id) {
                    towards.emplace_back((uint32_t)group.toward());
                }
            }
            auto exp_towards = helper::pattern_toward_list(type_id, pattern_id);
            EXPECT_EQ(towards.size(), exp_towards.size());
            for (uint32_t i = 0; i < towards.size(); ++i) {
                EXPECT_EQ(towards[i], exp_towards[i]);
            }
        }

        auto get_type_id = [](const auto g) { return g.type_id(); };
        const auto type_ids = groups | std::views::transform(get_type_id) | std::ranges::to<std::vector>();
        EXPECT_REPEAT(type_ids, group_union.unwrap());

        std::vector<int> pattern_ids {};
        for (auto group : groups) {
            if (pattern_ids.empty()) {
                pattern_ids.emplace_back(group.pattern_id());
                continue;
            }
            if (pattern_ids.back() != group.pattern_id()) {
                pattern_ids.emplace_back(group.pattern_id());
            }
        }
        EXPECT_IOTA(pattern_ids);

        std::vector<Group> result {};
        for (uint32_t pattern_id = 0; pattern_id < group_union.pattern_num(); ++pattern_id) {
            EXPECT_TRUE(group_union.groups(pattern_id).has_value());
            std::vector<Group::Toward> towards {};
            const auto sub_groups = group_union.groups(pattern_id).value();
            for (auto sub_group : sub_groups) {
                EXPECT_EQ(sub_group.type_id(), type_id);
                EXPECT_EQ(sub_group.pattern_id(), pattern_id);
                towards.emplace_back(sub_group.toward());
            }
            EXPECT_SORTED_AND_UNIQUE(towards);
            result.insert(result.end(), sub_groups.begin(), sub_groups.end());
        }
        EXPECT_FALSE(group_union.groups(-1).has_value());
        EXPECT_FALSE(group_union.groups(group_union.pattern_num()).has_value());

        EXPECT_EQ(result, group_union.groups());
    });
}

TEST(GroupUnion, constexpr) {
    static_assert(GroupUnion::create(169).has_value());
    static_assert(!GroupUnion::create(203).has_value());
    static_assert(GroupUnion::create(169).value().unwrap() == 169);

    constexpr auto group_union = GroupUnion::unsafe_create(169);
    static_assert(group_union.unwrap() == 169);

    static_assert(group_union.size() == 65880);
    static_assert(group_union.group_num() == 898);
    static_assert(group_union.pattern_num() == 230);
    static_assert(group_union.max_group_size() == 25955);

    static_assert(group_union == GroupUnion::unsafe_create(169));
    static_assert(group_union != GroupUnion::unsafe_create(203));

    static_assert(GroupUnion::from_raw_code(RawCode::unsafe_create(0x0603'EDF5'CAFF'F5E2)).unwrap() == 169);
    static_assert(GroupUnion::from_common_code(CommonCode::unsafe_create(0x1A9BF0C00)).unwrap() == 169);
}
