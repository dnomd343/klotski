/// Klotski Engine by Dnomd343 @2024

/// Group is a concept in klotski. For any valid cases, moving all its blocks
/// any finite number of times can generate a limited number of layouts, they
/// are called a `group`. Of course, there are some special groups whose size
/// is only 1, that is, only itself. (all blocks can no longer be moved)

/// For a case, by definition, it must have a `2x2` block, at least two spaces,
/// and the others are filled by any number of `1x1`, `1x2` and `2x1`, so their
/// numbers satisfy the following inequality.
///
///   => n_1x1 + (n_1x2 + n_2x1) * 2 + n_2x2 * 4 < (20 - 2)
///   => n_1x1 + (n_1x2 + n_2x1) * 2 < 14
///
/// Through calculation, it can be known that these three independent variables
/// can get 204 combinations. However, on a 5x4 chessboard, it's never possible
/// to put seven 2x1 blocks, so there are actually 203 combinations, and they
/// are numbered from 0 to 202, called `type_id`.

/// According to the number of blocks in the layout, you can use the following
/// formula to get an intermediate value `flag`, and arrange the flags in 203
/// cases from small to large to get the `type_id` value. Similarly, `type_id`
/// can also be reversed to get the number of blocks, which are one by one
/// corresponding.
///
///     flag  =>  |       xxx       |   xxx   |   xxxx   |
///   (10-bit)    | (n_1x2 + n_2x1) | (n_2x1) |  (n_1x1) |
///               |     (0 ~ 7)     | (0 ~ 7) | (0 ~ 14) |
///
///     flag  =>  ((n_1x2 + n_2x1) << 7) | (n_2x1 << 4) | (n_1x1)
///
/// Using the table lookup method, the `type_id` of any case can be obtained
/// within O(1), which is encapsulated in `GroupUnion`.

/// Since the `type_id` cannot change when moving, all cases belonging to the
/// same `type_id` must be divided into different groups (of course there may
/// be only one). For a group, list the CommonCodes of all its cases, the
/// smallest of which is called the group's `seed`. List all the groups under
/// the same `type_id`, and arrange them from large to small, and arrange the
/// groups of the same size from small to large according to the `seed`, then
/// start numbering from 0 to get the `group_id`.

/// All cases of the same group will have the same `type_id` and `group_id`,
/// that is to say, for cases with the same two values, there must be a
/// reachable path for them, otherwise they will never be reachable. Arrange
/// the CommonCodes of all cases in the group from small to large, and start
/// numbering from 0 to get `case_id`, which will uniquely determine a legal
/// layout. Use the following method to express.
///
///     {type_id}-{group_id}-{case_id}
///
///       Eg1: 1A9BF0C00 -> `169-1-7472`
///       Eg2: 4FEA13400 -> `164-0-30833`

/// The range of `type_id` is [0, 203), the maximum `group_id` is 2652 (there
/// are 2653 groups when `type_id = 164`), the maximum `case_id` is 964655
/// (there are 964656 cases when `type_id = 58` & `group_id = 0`). Therefore,
/// these three numbers meet the following range requirements.
///
///   | type_id: [0, 203) | group_id: [0, 2653) | case_id: [0, 964656) |
///   |   (8-bit ~ 256)   |   (12-bit ~ 4096)   |  (20-bit ~ 1048576)  |
///
/// Typically, these three variables are generally recorded in decimal and
/// displayed in the form of strings. They can facilitate the relationship
/// between multiple cases.

#pragma once

#include "raw_code/raw_code.h"
#include "short_code/short_code.h"
#include "common_code/common_code.h"

#include "internal/constant/group.h"
#include "internal/constant/group_union.h"

namespace klotski::group {

class Group;

class GroupUnion {
public:
    // ------------------------------------------------------------------------------------- //

    GroupUnion() = delete;

    /// Get the original type id.
    [[nodiscard]] constexpr uint_fast8_t unwrap() const;

    /// Create GroupUnion without any check.
    static constexpr GroupUnion unsafe_create(uint_fast8_t type_id);

    /// Create GroupUnion with validity check.
    static constexpr std::optional<GroupUnion> create(uint_fast8_t type_id);

    // ------------------------------------------------------------------------------------- //

    /// Get the number of cases contained.
    [[nodiscard]] constexpr uint32_t size() const;

    /// Get the number of groups contained.
    [[nodiscard]] constexpr uint32_t group_num() const;

    /// Get the number of patterns contained.
    [[nodiscard]] constexpr uint32_t pattern_num() const;

    /// Get the upper limit of the group size.
    [[nodiscard]] constexpr uint32_t max_group_size() const;

    // ------------------------------------------------------------------------------------- //

    using Groups = std::vector<Group>;

    /// Get all groups under the current type id.
    [[nodiscard]] constexpr Groups groups() const; // TODO: constexpr not support `std::vector`

    /// Get all klotski cases under the current type id.
    [[nodiscard]] cases::RangesUnion cases() const;

    /// Get the group instance with the specified pattern id.
    [[nodiscard]] constexpr std::optional<Groups> groups(uint_least16_t pattern_id) const;

    // ------------------------------------------------------------------------------------- //

    /// Create GroupUnion from RawCode.
    static constexpr GroupUnion from_raw_code(codec::RawCode raw_code);

    /// Create GroupUnion from ShortCode.
    static GroupUnion from_short_code(codec::ShortCode short_code);

    /// Create GroupUnion from CommonCode.
    static constexpr GroupUnion from_common_code(codec::CommonCode common_code);

    // ------------------------------------------------------------------------------------- //

#ifndef KLSK_NDEBUG
    /// Output type_id value only for debug.
    friend std::ostream& operator<<(std::ostream &out, GroupUnion self);
#endif

    /// Compare the type_id values of two GroupUnion.
    friend constexpr auto operator==(const GroupUnion &lhs, const GroupUnion &rhs);

    // ------------------------------------------------------------------------------------- //

private:
    uint_fast8_t type_id_;

    // ------------------------------------------------------------------------------------- //

    /// Get the type id of RawCode.
    static KLSK_INLINE_CE uint_fast8_t type_id(codec::RawCode raw_code);

    /// Get the type id of CommonCode.
    static KLSK_INLINE_CE uint_fast8_t type_id(codec::CommonCode common_code);

    // ------------------------------------------------------------------------------------- //
};

// TODO: using macro for all likely-class

static_assert(std::is_standard_layout_v<GroupUnion>);
static_assert(std::is_trivially_copyable_v<GroupUnion>);
static_assert(!std::is_default_constructible_v<GroupUnion>);
static_assert(std::is_trivially_destructible_v<GroupUnion>);
static_assert(std::is_nothrow_destructible_v<GroupUnion>);

static_assert(std::is_nothrow_copy_assignable_v<GroupUnion>);
static_assert(std::is_nothrow_move_assignable_v<GroupUnion>);
static_assert(std::is_nothrow_copy_constructible_v<GroupUnion>);
static_assert(std::is_nothrow_move_constructible_v<GroupUnion>);

static_assert(std::is_trivially_copy_assignable_v<GroupUnion>);
static_assert(std::is_trivially_move_assignable_v<GroupUnion>);
static_assert(std::is_trivially_copy_constructible_v<GroupUnion>);
static_assert(std::is_trivially_move_constructible_v<GroupUnion>);

class Group {
public:
    // ------------------------------------------------------------------------------------- //

    enum class Toward : uint_fast8_t {
        A = 0, // baseline
        B = 1, // horizontal mirror
        C = 2, // vertical mirror
        D = 3, // diagonal mirror
    };

    enum class MirrorType : uint_fast8_t {
        Full = 0, // fully self-symmetry
        Horizontal = 1, // horizontal self-symmetry
        Centro = 2, // centrosymmetric
        Vertical = 3, // vertical self-symmetry
        Ordinary = 4, // non self-symmetric
    };

    // ------------------------------------------------------------------------------------- //

    /// Get the mirror toward.
    [[nodiscard]] constexpr Toward toward() const;

    /// Get the toward character.
    [[nodiscard]] constexpr char toward_char() const;

    /// Get the original type id.
    [[nodiscard]] constexpr uint_fast8_t type_id() const;

    /// Get the original pattern id.
    [[nodiscard]] constexpr uint_fast16_t pattern_id() const;

    // ------------------------------------------------------------------------------------- //

    Group() = delete;

    /// Create Group without any check.
    static constexpr Group unsafe_create(uint_fast8_t type_id,
                                         uint_least16_t pattern_id, Toward toward);

    /// Create Group with validity check.
    static constexpr std::optional<Group> create(uint_fast8_t type_id,
                                                 uint_least16_t pattern_id, Toward toward);

    // ------------------------------------------------------------------------------------- //

    /// Get the number of cases contained.
    [[nodiscard]] constexpr uint32_t size() const;

    /// Get all klotski cases under current group.
    [[nodiscard]] cases::RangesUnion cases() const;

    // ------------------------------------------------------------------------------------- //

    /// Create Group from RawCode.
    static Group from_raw_code(codec::RawCode raw_code);

    /// Create Group from ShortCode.
    static Group from_short_code(codec::ShortCode short_code);

    /// Create Group from CommonCode.
    static Group from_common_code(codec::CommonCode common_code);

    // ------------------------------------------------------------------------------------- //

    /// Obtain the symmetry type of the group.
    [[nodiscard]] constexpr MirrorType mirror_type() const;

    /// Whether the group is vertically symmetrical.
    [[nodiscard]] constexpr bool is_vertical_mirror() const;

    /// Obtain the vertically symmetrical klotski group.
    [[nodiscard]] constexpr Group to_vertical_mirror() const;

    /// Whether the group is horizontally symmetrical.
    [[nodiscard]] constexpr bool is_horizontal_mirror() const;

    /// Obtain the horizontally symmetrical klotski group.
    [[nodiscard]] constexpr Group to_horizontal_mirror() const;

    // ------------------------------------------------------------------------------------- //

    /// Get the group in string form.
    [[nodiscard]] std::string to_string() const;

#ifndef KLSK_NDEBUG
    /// Output group info only for debug.
    friend std::ostream& operator<<(std::ostream &out, Group self);
#endif

    /// Compare the internal values of two Group.
    friend constexpr auto operator==(const Group &lhs, const Group &rhs);

    // ------------------------------------------------------------------------------------- //

private:
    Toward toward_;
    uint_fast8_t type_id_;
    uint_least16_t pattern_id_;

    /// Tiled merge of type_id and pattern_id.
    [[nodiscard]] constexpr uint32_t flat_id() const;

    /// Hidden constructor called from unsafe_create.
    constexpr Group(Toward toward, uint_fast8_t type_id, uint_least16_t pattern_id);

    // ------------------------------------------------------------------------------------- //
};

static_assert(std::is_standard_layout_v<Group>);
static_assert(std::is_trivially_copyable_v<Group>);

class CaseInfo {
public:
    // ------------------------------------------------------------------------------------- //

    /// Get the original group.
    [[nodiscard]] constexpr Group group() const;

    /// Get the original case id.
    [[nodiscard]] constexpr uint32_t case_id() const;

    // ------------------------------------------------------------------------------------- //

    CaseInfo() = delete;

    /// Create CaseInfo without any check.
    static constexpr CaseInfo unsafe_create(Group group, uint32_t case_id);

    /// Create CaseInfo with validity check.
    static constexpr std::optional<CaseInfo> create(Group group, uint32_t case_id);

    // ------------------------------------------------------------------------------------- //

    /// Get case info in string form.
    [[nodiscard]] std::string to_string() const;

#ifndef KLSK_NDEBUG
    /// Output case info only for debug.
    friend std::ostream& operator<<(std::ostream &out, CaseInfo self);
#endif

    /// Compare the internal values of two CaseInfo.
    friend constexpr auto operator==(const CaseInfo &lhs, const CaseInfo &rhs);

    // ------------------------------------------------------------------------------------- //

private:
    Group group_;
    uint32_t case_id_;

    /// Hidden constructor called from unsafe_create.
    constexpr CaseInfo(Group group, uint32_t case_id);

    // ------------------------------------------------------------------------------------- //
};

static_assert(std::is_standard_layout_v<CaseInfo>);
static_assert(std::is_trivially_copyable_v<CaseInfo>);

class GroupCases {
public:
    // ------------------------------------------------------------------------------------- //

    /// Execute the build process.
    static void build();

    /// Execute the build process without blocking.
    static void build_async(Executor &&executor, Notifier &&callback);

    // ------------------------------------------------------------------------------------- //

    /// Get the Group which ShortCode is located.
    static Group obtain_group(codec::ShortCode short_code);

    /// Get the Group which CommonCode is located.
    static Group obtain_group(codec::CommonCode common_code);

    // ------------------------------------------------------------------------------------- //

    /// Get the CommonCode from CaseInfo.
    static codec::CommonCode obtain_code(CaseInfo info);

    /// Get the CaseInfo corresponding to the ShortCode.
    static CaseInfo obtain_info(codec::ShortCode short_code);

    /// Get the CaseInfo corresponding to the CommonCode.
    static CaseInfo obtain_info(codec::CommonCode common_code);

    // ------------------------------------------------------------------------------------- //

private:
    // ------------------------------------------------------------------------------------- //

    /// Whether fast mode is available.
    static inline bool fast_ {false};

    /// Mutex for protecting critical section.
    static inline std::mutex busy_ {};

    // ------------------------------------------------------------------------------------- //

    /// Quickly obtain CommonCode from CaseInfo.
    static codec::CommonCode fast_obtain_code(CaseInfo info);

    /// Obtain CommonCode from CaseInfo without cache.
    static codec::CommonCode tiny_obtain_code(CaseInfo info);

    // ------------------------------------------------------------------------------------- //

    /// Quickly obtain Group from ShortCode.
    static Group fast_obtain_group(codec::ShortCode short_code);

    /// Quickly obtain Group from CommonCode.
    static Group fast_obtain_group(codec::CommonCode common_code);

    // ------------------------------------------------------------------------------------- //

    /// Quickly obtain CaseInfo from ShortCode.
    static CaseInfo fast_obtain_info(codec::ShortCode short_code);

    /// Quickly obtain CaseInfo from CommonCode.
    static CaseInfo fast_obtain_info(codec::CommonCode common_code);

    /// Obtain CaseInfo from CommonCode without cache.
    static CaseInfo tiny_obtain_info(codec::CommonCode common_code);

    // ------------------------------------------------------------------------------------- //
};

} // namespace klotski::group

#include "internal/type_id.inl"
#include "internal/group_union.inl"

#include "internal/group.inl"
#include "internal/group_mirror.inl"

#include "internal/group_cases.inl"
#include "internal/case_info.inl"
#include "internal/hash.inl"
