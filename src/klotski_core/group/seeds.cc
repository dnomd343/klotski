#include <algorithm>
#include "group.h"
#include "type_id.h"
#include "group_seeds.h"

#include <iostream>

namespace klotski {

CommonCode Group::group_seed(const CommonCode &common_code) {
    return group_seed(common_code.to_raw_code());
}

CommonCode Group::group_seed(const RawCode &raw_code) {
    auto cases = group_cases(raw_code);
    std::vector<CommonCode> group(cases.begin(), cases.end());
    return *std::min_element(group.begin(), group.end());
}

CommonCode Group::group_seed(const GroupId &group_id) {
    auto offset = TYPE_ID_OFFSET[group_id.type_id()]; // type id offset
    auto index = offset + GROUP_SEEDS_INDEX[offset + group_id.unwrap()];
    return CommonCode::unsafe_create(GROUP_SEEDS[index]);
}

std::vector<CommonCode> Group::group_seeds(const TypeId &type_id) {
    auto offset = GROUP_SEEDS + TYPE_ID_OFFSET[type_id.unwrap()]; // type id offset
    return {offset, offset + TYPE_ID_GROUP_NUM[type_id.unwrap()]};
}

} // namespace klotski
