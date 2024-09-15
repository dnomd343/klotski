#pragma once

#include "constant/group.h"

namespace klotski::cases {

// ----------------------------------------------------------------------------------------- //

constexpr uint32_t Group::size() const {
    return GROUP_SIZE[flat_id()];
}

constexpr uint32_t Group::type_id() const {
    return type_id_;
}

constexpr uint32_t Group::group_id() const {
    return group_id_;
}

constexpr uint32_t Group::flat_id() const {
    return GROUP_OFFSET[type_id_] + group_id_;
}

// ----------------------------------------------------------------------------------------- //

inline Group Group::from_common_code(codec::CommonCode common_code) {
    return from_raw_code(common_code.to_raw_code());
}

inline Group Group::from_short_code(codec::ShortCode short_code) {
    return from_common_code(short_code.to_common_code());
}

// ----------------------------------------------------------------------------------------- //

constexpr Group Group::unsafe_create(const uint32_t type_id, const uint32_t group_id) {
    return std::bit_cast<Group>(static_cast<uint64_t>(group_id) << 32 | type_id);
}

constexpr std::optional<Group> Group::create(const uint32_t type_id, const uint32_t group_id) {
    if (type_id < TYPE_ID_LIMIT && group_id < GROUP_NUM[type_id]) {
        return unsafe_create(type_id, group_id);
    }
    return std::nullopt;
}

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::cases
