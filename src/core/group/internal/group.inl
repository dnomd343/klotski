#pragma once

#include "constant/group.h"

namespace klotski::cases {

inline uint32_t Group::size() const {
    return GROUP_SIZE[flat_id()];
}

inline uint32_t Group::flat_id() const {
    return GROUP_OFFSET[type_id_] + group_id_;
}

inline uint32_t Group::type_id() const {
    return type_id_;
}

inline uint32_t Group::group_id() const {
    return group_id_;
}

inline Group Group::unsafe_create(const uint32_t type_id, const uint32_t group_id) {
    return std::bit_cast<Group>(static_cast<uint64_t>(group_id) << 32 | type_id);
}

inline std::optional<Group> Group::create(const uint32_t type_id, const uint32_t group_id) {
    if (type_id < TYPE_ID_LIMIT && group_id < GROUP_NUM[type_id]) {
        return unsafe_create(type_id, group_id);
    }
    return std::nullopt;
}

} // namespace klotski::cases
