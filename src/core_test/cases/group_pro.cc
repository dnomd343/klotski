#include <gtest/gtest.h>

#include "helper/group.h"
#include "group/group.h"

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
