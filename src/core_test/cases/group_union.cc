#include <gtest/gtest.h>

#include <cstdint>
#include <algorithm>

#include <ranges>

#include "group/group.h"
#include "helper/cases.h"
#include "common_code/common_code.h"

using klotski::codec::ShortCode;

using klotski::cases::Group;
using klotski::cases::GroupUnion;

using klotski::cases::TYPE_ID_LIMIT;
using klotski::cases::ALL_GROUP_NUM;

#define EXPECT_IOTA(R)           \
    EXPECT_FALSE(R.empty());     \
    EXPECT_EQ(R.front(), 0);     \
    EXPECT_SORTED_AND_UNIQUE(R); \
    EXPECT_EQ(R.back(), R.size() - 1)

TEST(GroupUnion, basic) {

    EXPECT_FALSE(GroupUnion::create(TYPE_ID_LIMIT).has_value());

    type_id_parallel([](uint32_t type_id) {

        EXPECT_TRUE(GroupUnion::create(type_id).has_value());
        EXPECT_EQ(GroupUnion::create(type_id).value().unwrap(), type_id);
        EXPECT_EQ(GroupUnion::unsafe_create(type_id).unwrap(), type_id);

        auto group_union = GroupUnion::unsafe_create(type_id);
        auto groups = group_union.groups();
        for (const auto group : groups) {
            EXPECT_EQ(group.type_id(), type_id);
        }

        auto group_ids = groups | std::views::transform([](Group g) {
            return g.group_id();
        }) | std::ranges::to<std::vector>();
        EXPECT_IOTA(group_ids);

        for (auto group_id : group_ids) {
            EXPECT_TRUE(group_union.group(group_id).has_value());
            EXPECT_EQ(group_union.group(group_id).value().type_id(), type_id);
            EXPECT_EQ(group_union.group(group_id).value().group_id(), group_id);
        }

        EXPECT_FALSE(group_union.group(group_ids.size()).has_value());

    });
}

TEST(GroupUnion, constant) {
    EXPECT_EQ(TYPE_ID_LIMIT, 203);
    EXPECT_EQ(ALL_GROUP_NUM, 25422);

    // TODO: verify from local builder

    // TODO: verify TYPE_ID_LIMIT / ALL_GROUP_NUM data

    // TODO: verify GROUP_UNION_SIZE (size) / GROUP_NUM (group_num) / MAX_GROUP_SIZE (max_group_size)
    //       test from member function directly?

    EXPECT_EQ(TYPE_ID_LIMIT, group_union_num());

}

TEST(GroupUnion, values) {

    type_id_parallel([](uint32_t type_id) {

        auto &kk = group_union_cases(type_id);
        EXPECT_EQ(GroupUnion::unsafe_create(type_id).size(), kk.size());
        EXPECT_EQ(GroupUnion::unsafe_create(type_id).group_num(), group_num(type_id));

        auto groups = GroupUnion::unsafe_create(type_id).groups();

        auto kk_view = groups | std::views::transform([](Group group) {
            return group.size();
        });

        auto max_it = *std::ranges::max_element(kk_view);

        EXPECT_EQ(GroupUnion::unsafe_create(type_id).max_group_size(), max_it);

    });

}

TEST(GroupUnion, cases) {
    type_id_parallel([](uint32_t type_id) {
        auto cases = group_union_cases(type_id);
        EXPECT_EQ(cases, GroupUnion::unsafe_create(type_id).cases().codes());
    });
}

TEST(GroupUnion, type_id) {
    ShortCode::speed_up(true);

    // TODO: using `common_code_parallel`
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto range : AllCases::instance().fetch()[head]) {
            auto common_code = CommonCode::unsafe_create(head << 32 | range);
            auto short_code = common_code.to_short_code();
            auto raw_code = common_code.to_raw_code();

            auto expect = to_type_id(cal_block_num(range));
            EXPECT_EQ(GroupUnion::from_raw_code(raw_code).unwrap(), expect);
            EXPECT_EQ(GroupUnion::from_short_code(short_code).unwrap(), expect);
            EXPECT_EQ(GroupUnion::from_common_code(common_code).unwrap(), expect);
        }
    }
}
