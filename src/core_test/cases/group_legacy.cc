#include <gtest/gtest.h>

#include "group/group.h"
#include "helper/cases.h"

//using klotski::cases::Group;
using klotski::cases::GroupUnion;

using klotski::cases::TYPE_ID_LIMIT;

// TODO: basic type_id / group_id verify with create / unsafe_create

// TODO: hash check for every group

//TEST(Group, cases) {
//
//    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
//        auto group_union = GroupUnion::unsafe_create(type_id);

//        for (auto group : group_union.groups()) {
//
//            auto &expect = group_cases(type_id, group.group_id());
//            auto codes = group.cases().codes();
//
//            EXPECT_EQ(codes, expect);
//
//            EXPECT_EQ(codes.size(), group.size());
//
//        }
//    }
//
//}

// TODO: test from_raw_code / from_short_code / from_common_code

// TODO: GroupCases hash verify
