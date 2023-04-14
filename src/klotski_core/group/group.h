#pragma once

#include <cstdint>
#include "raw_code.h"
#include "common_code.h"

namespace klotski {

const uint32_t TYPE_ID_LIMIT = 203;

class Group {
/// -------------------------------- block statistics ---------------------------------
public:
    /// 1. n_1x1 + (n_1x2 + n_2x1) * 2 <= 14
    /// 2. (n_1x1 != 0) && (n_2x1 != 7)
    struct block_num_t {
        uint8_t n_1x1 = 0; /// [0, 14]
        uint8_t n_1x2 = 0; /// [0, 7]
        uint8_t n_2x1 = 0; /// [0, 7]
    };

    /// Get type_id value (0 ~ 202).
    static uint32_t type_id(const RawCode &raw_code);
    static uint32_t type_id(const block_num_t &block_num);
    static uint32_t type_id(const CommonCode &common_code);

    /// Get the number of klotski blocks.
    static block_num_t block_num(uint32_t type_id);
    static block_num_t block_num(const RawCode &raw_code);
    static block_num_t block_num(const CommonCode &common_code);

/// --------------------------------- cases expansion ---------------------------------

    /// Search for all cases of the specified type_id.
    static std::vector<CommonCode> all_cases(uint32_t type_id);

    /// Search for all derivatives that a case can produce.
    static std::vector<RawCode> group_cases(const RawCode &raw_code);
    static std::vector<RawCode> group_cases(const CommonCode &common_code);

    // TODO: group_seed

    /// Calculate all groups in the specified type_id.
    static std::vector<std::vector<CommonCode>> build_groups(uint32_t type_id);

    // TODO: refactor build_group -> using GROUP_SEEDS

    /// Calculate the specified group using type_id and group_id.
    static std::vector<CommonCode> build_group(uint32_t type_id, uint32_t group_id);

/// ----------------------------------- group info ------------------------------------

    struct group_info_t {
        uint16_t type_id;
        uint16_t group_id;
        uint32_t group_index;
    };

    /// Get group info according to RawCode.
    static group_info_t group_info(const RawCode &raw_code);

    /// Get group info according to CommonCode.
    static group_info_t group_info(const CommonCode &common_code);

    /// Get the CommonCode according to the group info.
    static CommonCode group_case(const group_info_t &group_info);

    // TODO: group_size

/// -------------------------------- xxxxxxxxxxxxxxxxx --------------------------------

    // TODO: update max_group_size in TYPE_ID_MAX_GROUP

//    static uint32_t max_group_size(uint32_t type_id);

    static uint32_t max_group_size(const RawCode &raw_code) {
        return 65535 * 8;
    };

//    static uint32_t max_group_size(const CommonCode &common_code);

    // TODO: group_num

};

inline bool operator==(const Group::block_num_t &b1, const Group::block_num_t &b2) {
    return (b1.n_1x1 == b2.n_1x1) && (b1.n_1x2 == b2.n_1x2) && (b1.n_2x1 == b2.n_2x1);
}

inline bool operator!=(const Group::block_num_t &b1, const Group::block_num_t &b2) {
    return (b1.n_1x1 != b2.n_1x1) || (b1.n_1x2 != b2.n_1x2) || (b1.n_2x1 != b2.n_2x1);
}

}
