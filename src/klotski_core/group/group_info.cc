#include <algorithm>
#include "group.h"
#include "type_id.h"

namespace klotski {

/// --------------------------------------- Group Type ----------------------------------------

uint32_t GroupType::group_num() const noexcept {
    return TYPE_ID_GROUP_NUM[type_id_];
}

uint32_t GroupType::group_num(const RawCode &raw_code) noexcept {
    return GroupType(raw_code).group_num();
}

uint32_t GroupType::group_num(const CommonCode &common_code) noexcept {
    return GroupType(common_code).group_num();
}

uint32_t GroupType::max_size() const noexcept {
    return TYPE_ID_GROUP_MAX_SIZE[type_id_];
}

uint32_t GroupType::max_size(const RawCode &raw_code) noexcept {
    return GroupType(raw_code).max_size();
}

uint32_t GroupType::max_size(const CommonCode &common_code) noexcept {
    return GroupType(common_code).max_size();
}

/// --------------------------------------- Group Case ----------------------------------------

GroupCase::info_t GroupCase::encode(const RawCode &raw_code) noexcept {
    return encode(raw_code.to_common_code());
}

CommonCode GroupCase::parse(const info_t &info) {
    auto cases = Group(info.type_id, info.group_id).cases();
    if (cases.size() <= info.group_index) {
        throw std::invalid_argument("group index overflow"); // check group index
    }

    std::vector<CommonCode> group(cases.begin(), cases.end());
    std::nth_element(group.begin(), group.begin() + info.group_index, group.end());
    return group[info.group_index]; // located nth as target
}

GroupCase::info_t GroupCase::encode(const CommonCode &common_code) noexcept {
    uint32_t group_index = 0;
    auto cases = Group::cases(common_code);
    std::vector<CommonCode> group(cases.begin(), cases.end());
    for (auto &&code: group) {
        if (code < common_code) {
            ++group_index; // locate group index
        }
    }

    GroupType type = GroupType(common_code);
    auto seed = *std::min_element(group.begin(), group.end());
    return {
        .type_id = static_cast<uint16_t>(type.unwrap()),
        .group_id = static_cast<uint16_t>(group_id(type, seed)),
        .group_index = group_index,
    };
}

} // namespace klotski
