#include <stdexcept>
#include <algorithm>
#include "group.h"
#include "common.h"
#include "type_id.h"

namespace klotski {

using Common::range_reverse;

/// ----------------------------------------- Type ID -----------------------------------------

TypeId::TypeId(uint32_t type_id) {
    if (type_id >= TYPE_ID_LIMIT) { // invalid type id
        throw std::invalid_argument("type id overflow");
    }
    type_id_ = type_id;
}

TypeId::TypeId(const RawCode &raw_code) noexcept {
    type_id_ = to_type_id(block_num(raw_code));
}

TypeId::TypeId(const CommonCode &common_code) noexcept {
    type_id_ = to_type_id(block_num(common_code));
}

uint32_t TypeId::to_type_id(block_num_t &&block_num) noexcept {
    /// flag -> ... 0000  0xxx  0xxx  xxxx
    ///                  n_x2x n_2x1 n_1x1
    auto n_x2x = block_num.n_1x2 + block_num.n_2x1;
    auto flag = (n_x2x << 8) | (block_num.n_2x1 << 4) | block_num.n_1x1;
    return std::lower_bound(TYPE_ID_INDEX, TYPE_ID_INDEX + TYPE_ID_LIMIT, flag) - TYPE_ID_INDEX;
}

/// -------------------------------------- Block Number ---------------------------------------

TypeId::block_num_t TypeId::block_num() const noexcept {
    auto flag = TYPE_ID_INDEX[type_id_];
    auto n_2x1 = (flag >> 4) & 0b111;
    return block_num_t {
        .n_1x1 = static_cast<uint8_t>(flag & 0b1111),
        .n_1x2 = static_cast<uint8_t>((flag >> 8) - n_2x1),
        .n_2x1 = static_cast<uint8_t>(n_2x1),
    };
}

TypeId::block_num_t TypeId::block_num(const RawCode &raw_code) noexcept {
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

TypeId::block_num_t TypeId::block_num(const CommonCode &common_code) noexcept {
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

/// ---------------------------------------- Group ID -----------------------------------------

GroupId::GroupId(uint32_t type_id, uint32_t group_id) : type_id_(type_id) {
    if (group_id >= TYPE_ID_GROUP_NUM[type_id]) {
        throw std::invalid_argument("group id overflow");
    }
    group_id_ = group_id;
}

GroupId::GroupId(const TypeId &type_id, uint32_t group_id) : type_id_(type_id) {
    if (group_id >= TYPE_ID_GROUP_NUM[type_id.unwrap()]) {
        throw std::invalid_argument("group id overflow");
    }
    group_id_ = group_id;
}

} // namespace klotski
