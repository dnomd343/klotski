#pragma once

#include <cstdint>
#include "raw_code.h"
#include "common_code.h"

namespace klotski {

const uint32_t TYPE_ID_LIMIT = 203;

/// ----------------------------------------- Type ID -----------------------------------------

class TypeId {
public:
    /// 1. n_1x1 + (n_1x2 + n_2x1) * 2 <= 14
    /// 2. (n_1x1 != 0) && (n_2x1 != 7)
    struct block_num_t {
        uint8_t n_1x1 = 0; /// [0, 14]
        uint8_t n_1x2 = 0; /// [0, 7]
        uint8_t n_2x1 = 0; /// [0, 7]
    };
    /// n_space = 16 - n_1x1 - (n_1x2 + n_2x1) * 2

private:
    uint32_t type_id_;
    static inline uint32_t to_type_id(block_num_t &&block_num) noexcept;

public:
    explicit TypeId(uint32_t type_id);
    explicit TypeId(const RawCode &raw_code) noexcept;
    explicit TypeId(const CommonCode &common_code) noexcept;

    /// Release raw type id value.
    constexpr uint32_t unwrap() const noexcept { return type_id_; }

    /// Get the number of klotski blocks.
    block_num_t block_num() const noexcept;
    static block_num_t block_num(const RawCode &raw_code) noexcept;
    static block_num_t block_num(const CommonCode &common_code) noexcept;
};

inline bool operator==(const TypeId &t1, const TypeId &t2) {
    return t1.unwrap() == t2.unwrap();
}

inline bool operator!=(const TypeId &t1, const TypeId &t2) {
    return t1.unwrap() != t2.unwrap();
}

inline bool operator==(const TypeId::block_num_t &b1, const TypeId::block_num_t &b2) {
    return (b1.n_1x1 == b2.n_1x1) && (b1.n_1x2 == b2.n_1x2) && (b1.n_2x1 == b2.n_2x1);
}

inline bool operator!=(const TypeId::block_num_t &b1, const TypeId::block_num_t &b2) {
    return (b1.n_1x1 != b2.n_1x1) || (b1.n_1x2 != b2.n_1x2) || (b1.n_2x1 != b2.n_2x1);
}

/// ---------------------------------------- Group ID -----------------------------------------

class GroupId {
    TypeId type_id_;
    uint32_t group_id_;

public:
    GroupId(uint32_t type_id, uint32_t group_id);
    GroupId(const TypeId &type_id, uint32_t group_id);

    /// Release raw type id / group id value.
    constexpr uint32_t unwrap() const noexcept { return group_id_; }
    constexpr uint32_t type_id() const noexcept { return type_id_.unwrap(); }
};

inline bool operator==(const GroupId &g1, const GroupId &g2) {
    return g1.type_id() == g2.type_id() && g1.unwrap() == g2.unwrap();
}

inline bool operator!=(const GroupId &g1, const GroupId &g2) {
    return g1.type_id() != g2.type_id() || g1.unwrap() != g2.unwrap();
}

/// ------------------------------------------ Group ------------------------------------------

class Group {
public:
/// ----------------------------------- group seeds -----------------------------------

    /// Get the minimum CommonCode of the specified group.
    static CommonCode group_seed(const GroupId &group_id);
    static CommonCode group_seed(const RawCode &raw_code);
    static CommonCode group_seed(const CommonCode &common_code);

    /// Get all seeds in the specified type id.
    static std::vector<CommonCode> group_seeds(const TypeId &type_id);

/// --------------------------------- cases expansion ---------------------------------

    /// Search for all cases of the specified type_id.
    static std::vector<CommonCode> all_cases(const TypeId &type_id);

    /// Search for all derivatives that a case can produce.
    static std::vector<RawCode> group_cases(const RawCode &raw_code);
    static std::vector<RawCode> group_cases(const CommonCode &common_code);

    /// Calculate all groups in the specified type_id.
    static std::vector<std::vector<CommonCode>> build_groups(const TypeId &type_id);

    /// Calculate the specified group using type_id and group_id.
    static std::vector<CommonCode> build_group(const GroupId &group_id);

/// ----------------------------------- group info ------------------------------------

    struct group_info_t {
        uint16_t type_id;
        uint16_t group_id;
        uint32_t group_index;
    };

    /// Get group info according to specified case.
    static group_info_t group_info(const RawCode &raw_code);
    static group_info_t group_info(const CommonCode &common_code);

    /// Get the CommonCode according to the group info.
    static CommonCode group_case(const group_info_t &group_info);

    // TODO: group_size

/// -------------------------------- xxxxxxxxxxxxxxxxx --------------------------------

    // TODO: update max_group_size in TYPE_ID_MAX_GROUP

    static uint32_t group_max_size(const TypeId &type_id) {
        return 65535 * 8;
    }

    static uint32_t group_max_size(const RawCode &raw_code) {
        return group_max_size(TypeId(raw_code));
    }

    static uint32_t group_max_size(const CommonCode &common_code) {
        return group_max_size(TypeId(common_code));
    }

    // TODO: group_num

};

}
