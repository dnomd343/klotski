#pragma once

/// Group is a concept in klotski. For any case, moving all its blocks any finite
/// number of times can generate a limited number of layouts, they are called a
/// `group`. Of course, there are some special groups whose size is only `1`,
/// that is, only itself. (all blocks can no longer be moved)

/// For a case, by definition, it must have a `2x2` block, at least two spaces, and
/// the others are filled by any number of `1x1`, `1x2` and `2x1`, so their numbers
/// satisfy the following inequality.
///
///     n_1x1 + (n_1x2 + n_2x1) * 2 + n_2x2 * 4 < (20 - 2)
///       => n_1x1 + (n_1x2 + n_2x1) * 2 < 14
///
/// Through calculation, it can be known that these three independent variables can
/// get `204` combinations. However, on a `5x4` chessboard, it is never possible to
/// put seven `2x1` blocks, so there are actually `203` combinations, and they are
/// numbered from `0` to `202`, called `type_id`.

/// According to the number of blocks in the layout, you can use the following
/// formula to get an intermediate value `flag`, and arrange the flags in `203` cases
/// from small to large to get the `type_id` value. Similarly, `type_id` can also be
/// reversed to get the number of blocks, which are one by one corresponding.
///
///     flag =       0xxx      |   0xxx  |   xxxx    (12-bit)
///            (n_1x2 + n_2x1) | (n_2x1) | (n_1x1)
///                (0 ~ 7)     | (0 ~ 7) | (0 ~ 14)
///
///       => flag = ((n_1x2 + n_2x1) << 8) | (n_2x1 << 4) | (n_1x1)
///
/// Using the table lookup method, the `type_id` of any case can be obtained within
/// O(1), which is encapsulated in `GroupType`.

/// Since the `type_id` cannot change when moving, all cases belonging to the same
/// `type_id` must be divided into different groups (of course there may be only one).
/// For a group, list the CommonCodes of all its cases, the smallest of which is called
/// the group's `seed`. List all the groups under the same `type_id`, and arrange them
/// from large to small, and arrange the groups of the same size from small to large
/// according to the `seed`, and start numbering from `0` to get the `group_id`.

/// All cases of the same group will have the same `type_id` and `group_id`, that is
/// to say, for cases with the same two values, there must be a reachable path for them,
/// otherwise they will never be reachable. Arrange the CommonCodes of all cases in
/// the group from small to large, and start numbering from 0 to get `group_index`,
/// which will uniquely determine a legal layout. Use the following method to express.
///
///     {type_id}-{group_id}-{group_index}
///
///   Eg1: 1A9BF0C00 -> `169-1-7472`
///   Eg2: 4FEA13400 -> `164-0-30833`

/// The range of `type_id` is [0, 203), the maximum `group_id` is `2652` (there are
/// 2653 groups when `type_id` is 164), the maximum `group_index` is `964655` (there
/// are 964656 cases when `type_id` is 58 and `group_id` is 0). Therefore, these three
/// numbers meet the following range requirements.
///
///     type_id < 203  |  group_id < 2653  |  group_index < 964656
///     (8-bit ~ 256)  |  (12-bit ~ 4096)  |   (20-bit ~ 1048576)

#include <cstdint>
#include "raw_code.h"
#include "common_code.h"

namespace klotski {

const uint32_t TYPE_ID_LIMIT = 203;
const uint32_t ALL_GROUP_NUM = 25422;

/// --------------------------------------- Group Type ----------------------------------------

class GroupType {
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
    explicit GroupType(uint32_t type_id);
    explicit GroupType(const RawCode &raw_code) noexcept;
    explicit GroupType(const CommonCode &common_code) noexcept;

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
    uint32_t max_size() const noexcept;
    static uint32_t max_size(const RawCode &raw_code) noexcept;
    static uint32_t max_size(const CommonCode &common_code) noexcept;

    /// Get all seeds of the current type_id.
    std::vector<CommonCode> seeds() const noexcept;

    /// Search for all cases of the current type_id.
    std::vector<CommonCode> cases() const noexcept;

    /// Calculate all groups of the current type_id.
    std::vector<std::vector<CommonCode>> groups() const noexcept;
};

/// ------------------------------------------ Group ------------------------------------------

class Group {
    GroupType type_;
    uint32_t group_id_;

    static uint32_t group_id(GroupType group_type, CommonCode seed) noexcept;

public:
    Group(uint32_t type_id, uint32_t group_id);
    Group(const GroupType &group_type, uint32_t group_id);
    explicit Group(const RawCode &raw_code) noexcept;
    explicit Group(const CommonCode &common_code) noexcept;

    /// Release raw type_id / group_id value.
    constexpr uint32_t unwrap() const noexcept { return group_id_; }
    constexpr uint32_t type_id() const noexcept { return type_.unwrap(); }

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
    static uint32_t group_id(GroupType group_type, CommonCode seed) noexcept;

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

inline bool operator==(const GroupType &t1, const GroupType &t2) {
    return t1.unwrap() == t2.unwrap();
}

inline bool operator==(const Group &g1, const Group &g2) {
    return g1.type_id() == g2.type_id() && g1.unwrap() == g2.unwrap();
}

inline bool operator==(const GroupType::block_num_t &b1, const GroupType::block_num_t &b2) {
    return (b1.n_1x1 == b2.n_1x1) && (b1.n_1x2 == b2.n_1x2) && (b1.n_2x1 == b2.n_2x1);
}

inline bool operator==(const GroupCase::info_t &i1, const GroupCase::info_t &i2) {
    return (i1.type_id == i2.type_id) && (i1.group_id == i2.group_id) && (i1.group_index == i2.group_index);
}

inline bool operator!=(const Group &g1, const Group &g2) { return !(g1 == g2); }
inline bool operator!=(const GroupType &t1, const GroupType &t2) { return !(t1 == t2); }
inline bool operator!=(const GroupCase::info_t &i1, const GroupCase::info_t &i2) { return !(i1 == i2); }
inline bool operator!=(const GroupType::block_num_t &b1, const GroupType::block_num_t &b2) { return !(b1 == b2); }

} // namespace klotski
