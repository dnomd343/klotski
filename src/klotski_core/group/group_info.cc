#include <algorithm>
#include "group.h"
#include "type_id.h"
#include "group_seeds.h"

#include <iostream>

namespace klotski {

/// ----------------------------------------- Type ID -----------------------------------------

uint32_t TypeId::group_num() const noexcept {
    return TYPE_ID_GROUP_NUM[type_id_];
}

uint32_t TypeId::group_num(const RawCode &raw_code) noexcept {
    return TypeId(raw_code).group_num();
}

uint32_t TypeId::group_num(const CommonCode &common_code) noexcept {
    return TypeId(common_code).group_num();
}

uint32_t TypeId::group_max_size() const noexcept {
    return TYPE_ID_GROUP_MAX_SIZE[type_id_];
}

uint32_t TypeId::group_max_size(const RawCode &raw_code) noexcept {
    return TypeId(raw_code).group_max_size();
}

uint32_t TypeId::group_max_size(const CommonCode &common_code) noexcept {
    return TypeId(common_code).group_max_size();
}

/// ------------------------------------------ Group ------------------------------------------

Group::info_t Group::info(const RawCode &raw_code) {

    // TODO: function body
    return {};
}

Group::info_t Group::info(const CommonCode &common_code) {

    // TODO: function body

    return {};
}

CommonCode Group::resolve(const GroupId &group_id, uint32_t group_index) {

    // TODO: check group index

    // TODO: function body

    return CommonCode(0);
}

} // namespace klotski
