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

TEST(GroupUnion, basic) {

    // TODO: loop for all
    EXPECT_TRUE(GroupUnion::create(0).has_value());
    EXPECT_EQ(GroupUnion::create(0).value().unwrap(), 0);
    EXPECT_FALSE(GroupUnion::create(TYPE_ID_LIMIT).has_value());

    EXPECT_EQ(GroupUnion::unsafe_create(0).unwrap(), 0);

    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto group_union = GroupUnion::unsafe_create(type_id);

        auto num = group_union.group_num();

        auto groups = group_union.groups();
        for (auto x : groups) {
            EXPECT_EQ(x.type_id(), type_id);
        }
        // TODO: add EXPECT_IOTA(...)
//        auto group_ids = groups | std::ranges::transform([](Group x) { return x.group_id(); });
        std::vector<uint32_t> group_ids;
        group_ids.reserve(groups.size());
        for (auto x : groups) {
            group_ids.emplace_back(x.group_id());
        }
        EXPECT_SORTED_AND_UNIQUE(group_ids);
        EXPECT_EQ(group_ids[0], 0);
        EXPECT_EQ(group_ids[num - 1], num - 1);
        EXPECT_EQ(group_ids.size(), num);

        // TODO: test all
        EXPECT_TRUE(group_union.group(0).has_value());
        EXPECT_EQ(group_union.group(0).value().type_id(), type_id);
        EXPECT_EQ(group_union.group(0).value().group_id(), 0);

        EXPECT_FALSE(group_union.group(num).has_value());
    }

}

TEST(GroupUnion, constant) {
    EXPECT_EQ(TYPE_ID_LIMIT, 203);
    EXPECT_EQ(ALL_GROUP_NUM, 25422);

    // TODO: verify from local builder

    EXPECT_EQ(TYPE_ID_LIMIT, group_union_num());

}

TEST(GroupUnion, size) {
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto &kk = group_union_cases(type_id);
        EXPECT_EQ(GroupUnion::unsafe_create(type_id).size(), kk.size());
    }
}

TEST(GroupUnion, group_num) {
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        EXPECT_EQ(GroupUnion::unsafe_create(type_id).group_num(), group_num(type_id));
    }
}

TEST(GroupUnion, cases) {
    for (uint32_t type_id = 0; type_id < klotski::cases::TYPE_ID_LIMIT; ++type_id) {
        auto cases = group_union_cases(type_id);
        EXPECT_EQ(cases, GroupUnion::unsafe_create(type_id).cases().codes());
    }
}

// TODO: test max_group_size

TEST(GroupUnion, max_group_size) {
    for (uint32_t type_id = 0; type_id < klotski::cases::TYPE_ID_LIMIT; ++type_id) {

        auto groups = GroupUnion::unsafe_create(type_id).groups();

        auto sizes = groups | std::views::transform([](Group group) {
            return group.size();
        }) | std::ranges::to<std::vector>();

//        std::cout << std::format("{} -> {}\n", type_id, sizes);

        auto max_item = *std::max_element(sizes.begin(), sizes.end()); // TODO: using std::ranges

        EXPECT_EQ(GroupUnion::unsafe_create(type_id).max_group_size(), max_item);

    }
}

TEST(GroupUnion, type_id) {

    ShortCode::speed_up(true);

    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto range : AllCases::instance().fetch()[head]) {

            auto common_code = CommonCode::unsafe_create(head << 32 | range);
            auto short_code = common_code.to_short_code();
            auto raw_code = common_code.to_raw_code();

            auto expect = to_type_id(cal_block_num(range));

            EXPECT_EQ(GroupUnion::from_common_code(common_code).unwrap(), expect);
            EXPECT_EQ(GroupUnion::from_short_code(short_code).unwrap(), expect);
            EXPECT_EQ(GroupUnion::from_raw_code(raw_code).unwrap(), expect);

        }
    }

}
