#include <stdexcept>
#include <algorithm>
#include "group.h"
#include "common.h"
#include "type_id.h"

namespace klotski {

using Common::range_reverse;

/// --------------------------------------- Group Type ----------------------------------------

GroupType::GroupType(uint32_t type_id) {
    if (type_id >= TYPE_ID_LIMIT) { // invalid type id
        throw std::invalid_argument("type id overflow");
    }
    type_id_ = type_id;
}

GroupType::GroupType(const RawCode &raw_code) noexcept {
    type_id_ = type_id(block_num(raw_code));
}

GroupType::GroupType(const CommonCode &common_code) noexcept {
    type_id_ = type_id(block_num(common_code));
}

uint32_t GroupType::type_id(block_num_t &&block_num) noexcept { // block_num_t -> type_id
    /// flag -> ... 0000  0xxx  0xxx  xxxx
    ///                  n_x2x n_2x1 n_1x1
    auto n_x2x = block_num.n_1x2 + block_num.n_2x1;
    auto flag = (n_x2x << 8) | (block_num.n_2x1 << 4) | block_num.n_1x1;
    return std::lower_bound(TYPE_ID_INDEX, TYPE_ID_INDEX + TYPE_ID_LIMIT, flag) - TYPE_ID_INDEX;
}

/// -------------------------------------- Block Number ---------------------------------------

GroupType::block_num_t GroupType::block_num() const noexcept { // type_id -> block_num_t
    auto flag = TYPE_ID_INDEX[type_id_];
    auto n_2x1 = (flag >> 4) & 0b111;
    return block_num_t {
        .n_1x1 = static_cast<uint8_t>(flag & 0b1111),
        .n_1x2 = static_cast<uint8_t>((flag >> 8) - n_2x1),
        .n_2x1 = static_cast<uint8_t>(n_2x1),
    };
}

GroupType::block_num_t GroupType::block_num(const RawCode &raw_code) noexcept {
    block_num_t result;
    auto tmp = raw_code.unwrap();
    for (int addr = 0; addr < 20; ++addr, tmp >>= 3) {
        switch (tmp & 0b111) {
            case B_1x1:
                ++result.n_1x1;
                continue;
            case B_1x2:
                ++result.n_1x2;
                continue;
            case B_2x1:
                ++result.n_2x1;
                continue;
        }
    }
    return result;
}

GroupType::block_num_t GroupType::block_num(const CommonCode &common_code) noexcept {
    block_num_t result;
    auto range = range_reverse(static_cast<uint32_t>(common_code.unwrap()));
    for (; range; range >>= 2) {
        switch (range & 0b11) {
            case 0b01: /// 1x2 block
                ++result.n_1x2;
                continue;
            case 0b10: /// 2x1 block
                ++result.n_2x1;
                continue;
            case 0b11: /// 1x1 block
                ++result.n_1x1;
                continue;
        }
    }
    return result;
}

/// ------------------------------------------ Group ------------------------------------------

Group::Group(uint32_t type_id, uint32_t group_id) : type_(type_id) {
    if (group_id >= TYPE_ID_GROUP_NUM[type_id]) {
        throw std::invalid_argument("group id overflow");
    }
    group_id_ = group_id;
}

Group::Group(const GroupType &group_type, uint32_t group_id) : type_(group_type) {
    if (group_id >= TYPE_ID_GROUP_NUM[group_type.unwrap()]) {
        throw std::invalid_argument("group id overflow");
    }
    group_id_ = group_id;
}

Group::Group(const RawCode &raw_code) noexcept : type_(GroupType(raw_code)) {
    group_id_ = group_id(type_, Group::seed(raw_code));
}

Group::Group(const CommonCode &common_code) noexcept : type_(GroupType(common_code)) {
    group_id_ = group_id(type_, Group::seed(common_code));
}

} // namespace klotski
