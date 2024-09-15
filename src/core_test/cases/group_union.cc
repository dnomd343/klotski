#include <gtest/gtest.h>

#include <format>
#include <cstdint>
#include <algorithm>

#include <ranges>

// TODO: only for debug
#include <iostream>

#include "group/group.h"
#include "helper/cases.h"
#include "common_code/common_code.h"

using klotski::cases::Group;
using klotski::cases::GroupUnion;

using klotski::cases::TYPE_ID_LIMIT;
using klotski::cases::ALL_GROUP_NUM;

TEST(GroupUnion, basic) {

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
        EXPECT_EQ(cases, klotski::cases::GroupUnion::unsafe_create(type_id).cases().codes());
    }
}

TEST(GroupUnion, demo) {

    // for (auto i = 0; i < block_nums().size(); ++i) {
    //     std::cout << block_nums()[i].n_2x1 + block_nums()[i].n_1x2 << ", ";
    //     std::cout << block_nums()[i].n_2x1 << ", ";
    //     std::cout << block_nums()[i].n_1x1 << std::endl;
    // }

    std::vector<std::vector<uint64_t>> pp;

    pp.resize(block_nums().size());

    for (uint64_t head = 0; head < 16; ++head) {

        for (auto range : AllCases::instance().fetch()[head]) {
            uint64_t common_code = head << 32 | range;

            auto type_id = to_type_id(cal_block_num(common_code));

            pp[type_id].emplace_back(common_code);

        }

    }

    for (uint32_t type_id = 0; type_id < klotski::cases::TYPE_ID_LIMIT; ++type_id) {

        auto cases = klotski::cases::GroupUnion::unsafe_create(type_id).cases();

        std::vector<uint64_t> extend {};
        for (uint64_t head = 0; head < 16; ++head) {
            for (auto range : cases[head]) {
                extend.emplace_back(head << 32 | range);
            }
        }

        std::cout << "type_id " << type_id << " -> " << extend.size() << std::endl;

        EXPECT_EQ(extend, pp[type_id]);
    }


}
