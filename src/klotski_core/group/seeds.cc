#include <algorithm>
#include "group.h"
#include "type_id.h"
#include "group_seeds.h"

#include <iostream>

namespace klotski {

std::vector<CommonCode> TypeId::seeds() const noexcept {
    auto offset = GROUP_SEEDS + TYPE_ID_OFFSET[type_id_]; // type id offset
    return {offset, offset + TYPE_ID_GROUP_NUM[type_id_]};
}

CommonCode Group::seed() const noexcept { // group_id -> seed
    auto offset = TYPE_ID_OFFSET[type_id_.unwrap()]; // type id offset
    auto index = offset + GROUP_SEEDS_INDEX[offset + group_id_];
    return CommonCode::unsafe_create(GROUP_SEEDS[index]);
}

CommonCode Group::seed(const RawCode &raw_code) noexcept {
    auto cases = Group::cases(raw_code);
    std::vector<CommonCode> group(cases.begin(), cases.end());
    return *std::min_element(group.begin(), group.end());
}

CommonCode Group::seed(const CommonCode &common_code) noexcept {
    return seed(common_code.to_raw_code());
}

uint32_t Group::group_id(uint32_t type_id, const CommonCode &seed) noexcept { // seed -> group_id
    auto start = GROUP_SEEDS + TYPE_ID_OFFSET[type_id];
    auto end = start + TYPE_ID_GROUP_NUM[type_id];
    auto index = std::lower_bound(start, end, seed.unwrap()) - GROUP_SEEDS;
    return GROUP_SEEDS_INDEX_REV[index];
}

} // namespace klotski
