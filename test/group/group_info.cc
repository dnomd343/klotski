#include <algorithm>
#include "group.h"
#include "tiny_pool.h"
#include "gtest/gtest.h"

#define SHOULD_PANIC(FUNC) \
    try { \
        FUNC; EXPECT_STREQ("should panic", "but no panic"); \
    } catch (...) {}

using klotski::Group;
using klotski::GroupCase;
using klotski::GroupType;
using klotski::CommonCode;

using klotski::TYPE_ID_LIMIT;

TEST(Group, group_info_invalid) {
    SHOULD_PANIC(GroupCase::parse(GroupCase::info_t {
        .type_id = TYPE_ID_LIMIT,
        .group_id = 0,
        .group_index = 0,
    }))

    SHOULD_PANIC(GroupCase::parse(GroupCase::info_t {
        .type_id = 0,
        .group_id = static_cast<uint16_t>(GroupType(0).group_num()),
        .group_index = 0,
    }))

    SHOULD_PANIC(GroupCase::parse(GroupCase::info_t {
        .type_id = 0,
        .group_id = 0,
        .group_index = Group(0, 0).size(),
    }))
}

TEST(Group, group_info) {
    auto test = [](GroupType group_type) {
        for (uint32_t group_id = 0; group_id < group_type.group_num(); ++group_id) {
            auto tmp = Group(group_type, group_id).cases();
            std::vector<CommonCode> cases(tmp.begin(), tmp.end());

            auto min = *std::min_element(cases.begin(), cases.end());
            auto max = *std::max_element(cases.begin(), cases.end());

            auto min_info = GroupCase::info_t{
                .type_id = static_cast<uint16_t>(group_type.unwrap()),
                .group_id = static_cast<uint16_t>(group_id),
                .group_index = 0,
            };
            auto max_info = GroupCase::info_t{
                .type_id = static_cast<uint16_t>(group_type.unwrap()),
                .group_id = static_cast<uint16_t>(group_id),
                .group_index = static_cast<uint32_t>(cases.size() - 1),
            };

            EXPECT_EQ(min, GroupCase::parse(min_info));
            EXPECT_EQ(max, GroupCase::parse(max_info));
            EXPECT_EQ(min_info, GroupCase::encode(min));
            EXPECT_EQ(max_info, GroupCase::encode(max));
            EXPECT_EQ(min_info, GroupCase::encode(min.to_raw_code()));
            EXPECT_EQ(max_info, GroupCase::encode(max.to_raw_code()));
        }
    };

    auto pool = TinyPool();
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        pool.submit(test, GroupType(type_id));
    }
    pool.boot();
    pool.join();
}

// TODO: add group_info FAST mode
