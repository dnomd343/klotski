#include <gtest/gtest.h>

#include <ranges>
#include <cstdint>
#include <algorithm>

#include "helper/hash.h"
#include "helper/expect.h"
#include "helper/parallel.h"
#include "helper/block_num.h"

#include "helper/expect.h"

#include "helper/group.h"

#include "group/group.h"
#include "common_code/common_code.h"

using klotski::codec::ShortCode;

using klotski::group::Group;
using klotski::group::GroupUnion;

using klotski::group::TYPE_ID_LIMIT;
using klotski::group::ALL_GROUP_NUM;

using klotski::group::GROUP_NUM;
using klotski::group::PATTERN_NUM;

using klotski::group::ALL_GROUP_NUM;
using klotski::group::ALL_PATTERN_NUM;

using klotski::group::MAX_GROUP_SIZE;
using klotski::group::GROUP_UNION_SIZE;
using klotski::group::BLOCK_NUM;
using klotski::group::GROUP_UNION_CASES_NUM;

using klotski::group::GROUP_OFFSET;
using klotski::group::PATTERN_OFFSET;

EXPECT_PLAIN_DATA(GroupUnion); // TODO: using concept without macro
static_assert(helper::is_hashable_v<GroupUnion>);
static_assert(!std::is_default_constructible_v<GroupUnion>);

TEST(GroupUnion, basic) {
    TYPE_ID_PARALLEL({
        EXPECT_TRUE(GroupUnion::create(type_id).has_value());
        EXPECT_EQ(GroupUnion::create(type_id).value().unwrap(), type_id);
        EXPECT_EQ(GroupUnion::unsafe_create(type_id).unwrap(), type_id);
    });
    EXPECT_FALSE(GroupUnion::create(TYPE_ID_LIMIT).has_value());

    GROUP_UNION_PARALLEL({
        const auto groups = group_union.groups();
        EXPECT_EQ(groups.size(), group_union.group_num());

        auto get_type_id = [](const auto g) { return g.type_id(); };
        const auto type_ids = groups | std::views::transform(get_type_id) | std::ranges::to<std::vector>();
        EXPECT_REPEAT(type_ids, group_union.unwrap());

//        auto get_group_id = [](const auto g) { return g.group_id(); };
//        const auto group_ids = groups | std::views::transform(get_group_id) | std::ranges::to<std::vector>();
//        EXPECT_IOTA(group_ids);

//        for (const auto g : group_union.groups()) {
//            auto type_id = g.type_id();
//            auto group_id = g.group_id();
//            EXPECT_TRUE(group_union.group(group_id).has_value());
//            EXPECT_EQ(group_union.group(group_id)->type_id(), type_id);
//            EXPECT_EQ(group_union.group(group_id)->group_id(), group_id);
//        }
//        EXPECT_FALSE(group_union.group(group_union.group_num()).has_value());
    });
}

#define EXPECT_SUM_EQ(ARR, SUM) \
    EXPECT_EQ(std::accumulate(ARR.begin(), ARR.end(), 0), SUM)

#define EXPECT_OFFSET(ARR, OFFSET) \
    EXPECT_EQ(ARR.size(), OFFSET.size()); \
    EXPECT_FALSE(ARR.empty()) \
    // TODO: offset test

TEST(GroupUnion, constant) {

    EXPECT_EQ(TYPE_ID_LIMIT, helper::group_union_num());

    // GROUP_NUM / ALL_GROUP_NUM
    // PATTERN_NUM / ALL_PATTERN_NUM
    // MAX_GROUP_SIZE
    // GROUP_UNION_SIZE
    // BLOCK_NUM
    // GROUP_UNION_CASES_NUM

    EXPECT_EQ(GROUP_NUM.size(), TYPE_ID_LIMIT);
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) { // TODO: add `TYPE_ID_LOOP` macro
        EXPECT_EQ(GROUP_NUM[type_id], helper::group_union_group_num(type_id));
    }
    EXPECT_OFFSET(GROUP_NUM, GROUP_OFFSET);
    EXPECT_SUM_EQ(GROUP_NUM, ALL_GROUP_NUM);

    EXPECT_EQ(PATTERN_NUM.size(), TYPE_ID_LIMIT);
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        EXPECT_EQ(PATTERN_NUM[type_id], helper::group_union_pattern_num(type_id));
    }
    EXPECT_OFFSET(PATTERN_NUM, PATTERN_OFFSET);
    EXPECT_SUM_EQ(PATTERN_NUM, ALL_PATTERN_NUM);

    EXPECT_EQ(MAX_GROUP_SIZE.size(), TYPE_ID_LIMIT);
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        const auto sizes = GroupUnion::unsafe_create(type_id).groups()
            | std::views::transform([](const auto g) { return g.size(); });
        EXPECT_EQ(MAX_GROUP_SIZE[type_id], *std::ranges::max_element(sizes)); // TODO: maybe test with `max_group_size()`
    }

    static_assert(GROUP_UNION_SIZE.size() == TYPE_ID_LIMIT);
    // TODO: test GROUP_UNION_SIZE

    static_assert(BLOCK_NUM.size() == TYPE_ID_LIMIT);
    for (auto type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto [n, n_2x1, n_1x1] = BLOCK_NUM[type_id];
        auto val = helper::cal_block_num(type_id);
        EXPECT_EQ(n, val.n_1x2 + val.n_2x1);
        EXPECT_EQ(n_2x1, val.n_2x1);
        EXPECT_EQ(n_1x1, val.n_1x1);
    }

    static_assert(GROUP_UNION_CASES_NUM.size() == TYPE_ID_LIMIT);
    for (auto type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto [a, b, c, d] = GROUP_UNION_CASES_NUM[type_id];

        auto cases = GroupUnion::unsafe_create(type_id).cases();
        EXPECT_EQ(a, cases.ranges(0).size());
        // TODO: move value compare
    }

}

TEST(GroupUnion, values) {
    GROUP_UNION_PARALLEL({
        const auto type_id = group_union.unwrap();
        EXPECT_EQ(group_union.size(), helper::group_union_cases(type_id).size());
        EXPECT_EQ(group_union.group_num(), helper::group_union_group_num(type_id));
        EXPECT_EQ(group_union.pattern_num(), helper::group_union_pattern_num(type_id));

        const auto sizes = group_union.groups()
                         | std::views::transform([](const auto g) { return g.size(); });
        EXPECT_EQ(group_union.max_group_size(), *std::ranges::max_element(sizes));
    });
}

TEST(GroupUnion, cases) {
    GROUP_UNION_PARALLEL({
        const auto type_id = group_union.unwrap();
        auto &cases = helper::group_union_cases(type_id);

        EXPECT_EQ(group_union.cases().codes(), cases);
    });
    // TODO: combine all cases and compare with AllCases
}

// TODO: add `EXPECT_DIFF(...)` helper

TEST(GroupUnion, groups) {
    GROUP_UNION_PARALLEL({
        const auto type_id = group_union.unwrap();

        auto groups = group_union.groups();
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
        // TODO: test two `groups()` interfaces
    });
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
