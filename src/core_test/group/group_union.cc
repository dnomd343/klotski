#include <gtest/gtest.h>

#include <ranges>
#include <cstdint>
#include <algorithm>

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

static_assert(std::is_standard_layout_v<GroupUnion>);
static_assert(std::is_trivially_copyable_v<GroupUnion>);
static_assert(!std::is_default_constructible_v<GroupUnion>);
static_assert(std::is_trivially_destructible_v<GroupUnion>);
static_assert(std::is_nothrow_destructible_v<GroupUnion>);

static_assert(std::is_nothrow_copy_assignable_v<GroupUnion>);
static_assert(std::is_nothrow_move_assignable_v<GroupUnion>);
static_assert(std::is_nothrow_copy_constructible_v<GroupUnion>);
static_assert(std::is_nothrow_move_constructible_v<GroupUnion>);

static_assert(std::is_trivially_copy_assignable_v<GroupUnion>);
static_assert(std::is_trivially_move_assignable_v<GroupUnion>);
static_assert(std::is_trivially_copy_constructible_v<GroupUnion>);
static_assert(std::is_trivially_move_constructible_v<GroupUnion>);

#define EXPECT_REPEAT(R, val)  \
    EXPECT_FALSE(R.empty());   \
    EXPECT_EQ(R.front(), val); \
    EXPECT_EQ(std::adjacent_find(R.begin(), R.end(), std::not_equal_to<>()), R.end())

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

TEST(GroupUnion, constant) {

    EXPECT_EQ(TYPE_ID_LIMIT, helper::group_union_num());

    uint32_t sum = 0;
    for (uint32_t i = 0; i < helper::group_union_num(); ++i) {
        sum += helper::group_union_group_num(i);
    }
    EXPECT_EQ(ALL_GROUP_NUM, sum);

    // TODO: verify GROUP_UNION_SIZE (size) / GROUP_NUM (group_num) / MAX_GROUP_SIZE (max_group_size)
    //       test from member function directly?
}

// TEST(GroupUnion, values) {
//     GROUP_UNION_PARALLEL({
//         auto type_id = group_union.unwrap();
//         auto &cases = group_union_cases(type_id);
//
//         EXPECT_EQ(group_union.size(), cases.size());
//         EXPECT_EQ(group_union.cases().codes(), cases);
//         EXPECT_EQ(group_union.group_num(), helper::group_union_group_num(type_id));
//
//         auto get_group_size = [](auto g) { return g.size(); };
//         const auto sizes = group_union.groups() | std::views::transform(get_group_size);
//         EXPECT_EQ(group_union.max_group_size(), *std::ranges::max_element(sizes));
//     });
// }

TEST(GroupUnion, values) {
    GROUP_UNION_PARALLEL({
        auto type_id = group_union.unwrap();
        auto &cases = helper::group_union_cases(type_id);

        EXPECT_EQ(group_union.size(), cases.size());
        EXPECT_EQ(group_union.cases().codes(), cases);
        EXPECT_EQ(group_union.group_num(), helper::group_union_group_num(type_id));
        EXPECT_EQ(group_union.pattern_num(), helper::group_union_pattern_num(type_id));

        auto get_group_size = [](auto g) { return g.size(); };
        const auto sizes = group_union.groups() | std::views::transform(get_group_size);
        EXPECT_EQ(group_union.max_group_size(), *std::ranges::max_element(sizes));

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
    });
}

TEST(GroupUnion, type_id) {
    ShortCode::speed_up(true);
    COMMON_CODE_PARALLEL({
         auto type_id = to_type_id(helper::cal_block_num(code.unwrap()));
         EXPECT_EQ(GroupUnion::from_common_code(code).unwrap(), type_id);
         EXPECT_EQ(GroupUnion::from_raw_code(code.to_raw_code()).unwrap(), type_id);
         EXPECT_EQ(GroupUnion::from_short_code(code.to_short_code()).unwrap(), type_id);
    });
}
