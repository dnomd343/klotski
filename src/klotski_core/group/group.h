#pragma once

#include <cstdint>
#include "raw_code.h"
#include "common_code.h"

namespace klotski {

const uint32_t TYPE_ID_LIMIT = 203;
const uint32_t ALL_GROUP_NUM = 25422;

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
    static inline uint32_t type_id(block_num_t &&block_num) noexcept;

public:
    explicit TypeId(uint32_t type_id);
    explicit TypeId(const RawCode &raw_code) noexcept;
    explicit TypeId(const CommonCode &common_code) noexcept;

    /// Release raw type_id value.
    constexpr uint32_t unwrap() const noexcept { return type_id_; }

    /// Get the number of klotski blocks.
    block_num_t block_num() const noexcept;
    static block_num_t block_num(const RawCode &raw_code) noexcept;
    static block_num_t block_num(const CommonCode &common_code) noexcept;

    /// Get the number of groups.
    uint32_t group_num() const noexcept;
    static uint32_t group_num(const RawCode &raw_code) noexcept;
    static uint32_t group_num(const CommonCode &common_code) noexcept;

    /// Get the max group size.
    uint32_t group_max_size() const noexcept;
    static uint32_t group_max_size(const RawCode &raw_code) noexcept;
    static uint32_t group_max_size(const CommonCode &common_code) noexcept;

    /// Get all seeds of the current type_id.
    std::vector<CommonCode> seeds() const noexcept;

    /// Search for all cases of the current type_id.
    std::vector<CommonCode> cases() const noexcept;

    /// Calculate all groups of the current type_id.
    std::vector<std::vector<CommonCode>> groups() const noexcept;
};

/// ------------------------------------------ Group ------------------------------------------

class Group {
    TypeId type_id_;
    uint32_t group_id_;

    static uint32_t group_id(uint32_t type_id, const CommonCode &seed) noexcept;

public:
    Group(uint32_t type_id, uint32_t group_id);
    Group(const TypeId &type_id, uint32_t group_id);
    explicit Group(const RawCode &raw_code) noexcept;
    explicit Group(const CommonCode &common_code) noexcept;

    /// Release raw type_id / group_id value.
    constexpr uint32_t unwrap() const noexcept { return group_id_; }
    constexpr uint32_t type_id() const noexcept { return type_id_.unwrap(); }

    /// Get the size of the current group.
    uint32_t size() const noexcept;
    static uint32_t size(const RawCode &raw_code) noexcept;
    static uint32_t size(const CommonCode &common_code) noexcept;

    /// Get the minimum CommonCode.
    CommonCode seed() const noexcept;
    static CommonCode seed(const RawCode &raw_code) noexcept;
    static CommonCode seed(const CommonCode &common_code) noexcept;

    /// Calculate the current group.
    std::vector<RawCode> cases() const noexcept;
    static std::vector<RawCode> cases(const RawCode &raw_code) noexcept;
    static std::vector<RawCode> cases(const CommonCode &common_code) noexcept;
};

/// --------------------------------------- Group Case ----------------------------------------

class GroupCase {
public:
    struct info_t {
        uint16_t type_id;
        uint16_t group_id;
        uint32_t group_index;
    };

    /// Get the CommonCode using the group info.
    static CommonCode parse(const info_t &info);

    /// Get group info according to specified case.
    static info_t encode(const RawCode &raw_code) noexcept;
    static info_t encode(const CommonCode &common_code) noexcept;
};

/// ---------------------------------------- Operators ----------------------------------------

inline bool operator==(const TypeId &t1, const TypeId &t2) {
    return t1.unwrap() == t2.unwrap();
}

inline bool operator==(const Group &g1, const Group &g2) {
    return g1.type_id() == g2.type_id() && g1.unwrap() == g2.unwrap();
}

inline bool operator==(const TypeId::block_num_t &b1, const TypeId::block_num_t &b2) {
    return (b1.n_1x1 == b2.n_1x1) && (b1.n_1x2 == b2.n_1x2) && (b1.n_2x1 == b2.n_2x1);
}

inline bool operator==(const GroupCase::info_t &i1, const GroupCase::info_t &i2) {
    return (i1.type_id == i2.type_id) && (i1.group_id == i2.group_id) && (i1.group_index == i2.group_index);
}

inline bool operator!=(const Group &g1, const Group &g2) { return !(g1 == g2); }
inline bool operator!=(const TypeId &t1, const TypeId &t2) { return !(t1 == t2); }
inline bool operator!=(const GroupCase::info_t &i1, const GroupCase::info_t &i2) { return !(i1 == i2); }
inline bool operator!=(const TypeId::block_num_t &b1, const TypeId::block_num_t &b2) { return !(b1 == b2); }

} // namespace klotski
