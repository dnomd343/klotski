/// Klotski Engine by Dnomd343 @2024

/// Group is a concept in klotski. For any valid cases, moving any block in any
/// finite number of times can generate a limited number of cases, we call them
/// `Group`. Of course, there are some special groups whose size is only one,
/// that is, only itself. (all blocks can no longer be moved)

/// For a case, by definition, it must have a `2x2` block, at least two spaces,
/// and the others are filled by any number of `1x1`, `1x2` and `2x1`, so their
/// numbers satisfy the following inequality.
///
///   => n_1x1 + (n_1x2 + n_2x1) * 2 + n_2x2 * 4 <= (20 - 2)
///   => n_1x1 + (n_1x2 + n_2x1) * 2 <= 14
///
/// Through calculation, it can be known that these three independent variables
/// can get 204 combinations. However, on a 5x4 chessboard, it's never possible
/// to put seven 2x1 blocks, so there are actually 203 combinations, and they
/// are numbered from 0 to 202, called `type_id`.

/// According to the number of blocks in the layout, you can use the following
/// formula to get an intermediate value `flag`, and arrange the flags in 203
/// cases from small to large that get the type_id values. Similarly, `type_id`
/// can also be used to obtain the number of blocks, which are corresponded one
/// by one.
///
///               ┏━━━━━━━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━━┓
///     flag   => ┃      xxx      ┃   xxx   ┃   xxxx   ┃
///   (10-bit)    ┃ n_1x2 + n_2x1 ┃  n_2x1  ┃  n_1x1   ┃
///               ┃    (0 ~ 7)    ┃ (0 ~ 7) ┃ (0 ~ 14) ┃
///               ┗━━━━━━━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━━┛
///            =>  ((n_1x2 + n_2x1) << 7) | (n_2x1 << 4) | (n_1x1)
///
/// Using the table lookup method, the `type_id` of any case can be obtained
/// within O(1), which is encapsulated in `GroupUnion`.

/// Since the `type_id` cannot change when moving, all cases belonging to the
/// same `type_id` must be divided into different groups (of course there may
/// be only one). For a group, list the CommonCodes of all its cases, the
/// smallest of which is called the group's `seed`.

/// Consider selecting any case from a group, if its horizontally symmetrical
/// case is also in this group, we say it satisfies horizontal self-symmetry.
/// It is easy to prove that the horizontally symmetrical case of each element
/// in this group can be found in itself (of course, the case itself may be
/// horizontally symmetrical). If a group is not horizontally self-symmetrical,
/// there must be another group that is the horizontal mirror of it. We can
/// understand that the existence of horizontal self-symmetry is because these
/// two groups are "connected" and combined into one group.

/// Similarly, we can define vertical self-symmetry and centrosymmetric. The
/// latter is more special, any case of it can be found after rotating 180
/// degrees (i.e., performing horizontal and vertical flipping). If a group
/// satisfies both horizontal self-symmetry and vertical self-symmetry, we call
/// it fully self-symmetry, it must also satisfy centrosymmetric. That is, the
/// symmetrical case of any element, no matter how, the result is within the
/// scope of the group. Therefore, we can distinguish the following five
/// MirrorType attributes:
///
///   1. `Full`: fully self-symmetry group
///   2. `Horizontal`: horizontal self-symmetry group
///   3. `Centro`: centrosymmetric group
///   4. `Vertical`: vertical self-symmetry group
///   5. `Ordinary`: non self-symmetric group
///
/// Note that for the horizontal self-symmetry group, it does not satisfy the
/// vertical self-symmetry and centrosymmetric. The same is true for others,
/// any group is one of the above 5 types.

/// Furthermore, for a horizontal self-symmetry group, if we vertically mirror
/// all of its cases, we will get another group with the same MirrorType, which
/// are vertically symmetric to each other. Although they are not same group,
/// their properties are consistent or mirrored, which we call them `Pattern`.
/// Similarly, there is another group in same pattern that is symmetric to the
/// vertical self-symmetry group and the centrosymmetric group. For these three
/// cases, each pattern contains 2 groups. For the fully self-symmetry group,
/// it itself is a pattern, that is, it only contains one group. For the non
/// self-symmetric group, it can be horizontally mirrored, vertically mirrored,
/// and centrally rotated to obtain another three groups, and the pattern they
/// form contains 4 groups.

/// In a pattern, there may be 1/2/4 groups, each with a different toward. The
/// group with the smallest `seed` is defined as A, whose horizontal symmetry
/// group is B, vertical symmetry group is C, and group rotated 180 degrees is
/// D. The above 5 types of situations correspond to 5 types of patterns. These
/// four towards may overlap, if so, the smallest toward is taken. In pattern
/// type 1, the four towards A/B/C/D are interconnected, which means that the
/// only group toward is A. In pattern type 2, A and B are interconnected, and
/// C and D are also interconnected. Therefore, the groups containing the two
/// towards A and C are vertically symmetric to each other. Similarly, the
/// pattern type 4 contains the two directions A and B, which are horizontally
/// symmetric to each other. The pattern type 3 also contains the two towards A
/// and B, they are both horizontally and vertically symmetric to each other.
/// As for the pattern type 5, it contains the four towards A/B/C/D. The 5
/// patterns correspond to the following toward combinations:
///
///   1. fully self-symmetry group      x1 -> A (=B=C=D)
///   2. horizontal self-symmetry group x2 -> A (=B) + C (=D)
///   3. centrosymmetric group          x2 -> A (=D) + B (=C)
///   4. vertical self-symmetry group   x2 -> A (=C) + B (=D)
///   5. non-self-symmetric group       x4 -> A + B + C + D

/// In the same type_id, there may be multiple patterns, which are prioritized
/// according to the following three conditions and given numbers starting from
/// zero, namely `pattern_id`:
///
///   1. The number of cases contained in pattern, from large to small.
///   2. The pattern type of the above five from small to large.
///   3. The smallest `seed` of the groups in pattern, from small to large.
///
/// Therefore, each group can be uniquely identified by type_id, pattern_id and
/// toward. The first two are numbers, and the last is an enumeration. In order
/// to easily identify the symmetry type of pattern, they will be represented
/// by different characters:
///
///   1. `x` (Toward::A)
///   2. `n` (Toward::A) / `u` (Toward::C)
///   3. `s` (Toward::A) / `o` (Toward::B)
///   4. `p` (Toward::A) / `q` (Toward::B)
///   5. `a` (Toward::A) / `b` (Toward::B) / `c` (Toward::C) / `d` (Toward::D)
///
/// That is to say, we can use the format of `{}-{}_` to represent a group. For
/// example, the group of `1A9BF0C00`, its type_id is 169, pattern_id is 0, and
/// toward is C, which is recorded as `169-0u`, while the group of `4FEA13400`
/// is recorded as `164-0x`.

/// All cases of the group will have the same type_id, pattern_id and toward,
/// that is to say, for cases with the same values, there must be a reachable
/// path for them, otherwise they will never be reachable. Arrange all the
/// CommonCodes in group from small to large, and start numbering from 0 to get
/// `case_id`, which will uniquely determine a legal case. Use the following
/// method to express:
///
///   {type_id}-{pattern_id}{toward_char}-{case_id}
///
///   Eg1: 1A9BF0C00 -> `169-0u-7472`
///   Eg2: 4FEA13400 -> `164-0x-30833`

/// The value ranges of these four key variables are as follows:
///
///   + type_id:    [0, 203)    (8-bit ~ 256)
///   + pattern_id: [0, 665)    (10-bit ~ 1024)    (max in type_id `164`)
///   + toward:     [0, 4)      (2-bit ~ 4)
///   + case_id:    [0, 964656) (20-bit ~ 1048576) (max in group `58-0x`)
///
/// These four variables are collectively called CaseInfo, and they correspond
/// uniquely to each case. We can quickly get the type_id value of CommonCode,
/// but getting other variables requires a lot of calculations. Coincidentally,
/// they can be stored in a 32-bit length, which means that we can store all
/// case information in advance to quickly convert CommonCode and CaseInfo.

#pragma once

#include <mutex>

#include "group/group_fwd.h"
#include "ranges/ranges_fwd.h"
#include "raw_code/raw_code_fwd.h"
#include "short_code/short_code_fwd.h"
#include "common_code/common_code_fwd.h"

#include "internal/constant/group.h"
#include "internal/constant/group_union.h"

// TODO: using `uint_fast8_t` is no needed (just using `uint8_t`)

namespace klotski::group {

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
    [[nodiscard]] Groups groups() const;

    /// Get all klotski cases under the current type id.
    [[nodiscard]] cases::RangesUnion cases() const;

    /// Get the group instance with the specified pattern id.
    [[nodiscard]] std::optional<Groups> groups(uint_least16_t pattern_id) const;

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

static_assert(sizeof(GroupUnion) == 1);
static_assert(is_compact_layout_v<GroupUnion>);
static_assert(std::is_standard_layout_v<GroupUnion>);
static_assert(std::is_trivially_copyable_v<GroupUnion>);

} // namespace klotski::group

template <>
struct std::hash<klotski::group::GroupUnion> {
    constexpr std::size_t operator()(const klotski::group::GroupUnion &gu) const noexcept {
        return std::hash<uint_fast8_t>{}(gu.unwrap());
    }
};

namespace klotski::group {

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
        Ordinary = 4, // non-self-symmetric
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
    [[nodiscard]] KLSK_INLINE std::string to_string() const;

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

static_assert(sizeof(Group) == 4);
static_assert(is_compact_layout_v<Group>);
static_assert(std::is_standard_layout_v<Group>);
static_assert(std::is_trivially_copyable_v<Group>);

} // namespace klotski::group

template <>
struct std::hash<klotski::group::Group> {
    constexpr std::size_t operator()(const klotski::group::Group &g) const noexcept {
        return std::hash<uint32_t>{}(std::bit_cast<uint32_t>(g));
    }
};

namespace klotski::group {

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
    [[nodiscard]] KLSK_INLINE std::string to_string() const;

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

static_assert(sizeof(CaseInfo) == 8);
static_assert(is_compact_layout_v<CaseInfo>);
static_assert(std::is_standard_layout_v<CaseInfo>);
static_assert(std::is_trivially_copyable_v<CaseInfo>);

} // namespace klotski::group

template <>
struct std::hash<klotski::group::CaseInfo> {
    constexpr std::size_t operator()(const klotski::group::CaseInfo &info) const noexcept {
        return std::hash<uint64_t>{}(std::bit_cast<uint64_t>(info));
    }
};

namespace klotski::group {

class GroupCases {
public:
    // ------------------------------------------------------------------------------------- //

    /// Execute the build process.
    static void build();

    /// Execute the build process without blocking.
    static void build_async(Executor &&executor, Notifier &&callback);

    static bool is_fast_mode() {
        return fast_;
    }

    // ------------------------------------------------------------------------------------- //

    /// Get the CommonCode from CaseInfo.
    static KLSK_INLINE codec::CommonCode obtain_code(CaseInfo info);

    // ------------------------------------------------------------------------------------- //

    /// Get the Group which ShortCode is located.
    static KLSK_INLINE Group obtain_group(codec::ShortCode short_code);

    /// Get the Group which CommonCode is located.
    static KLSK_INLINE Group obtain_group(codec::CommonCode common_code);

    // ------------------------------------------------------------------------------------- //

    /// Get the CaseInfo corresponding to the ShortCode.
    static KLSK_INLINE CaseInfo obtain_info(codec::ShortCode short_code);

    /// Get the CaseInfo corresponding to the CommonCode.
    static KLSK_INLINE CaseInfo obtain_info(codec::CommonCode common_code);

    // ------------------------------------------------------------------------------------- //

private:
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

#include "internal/group.inl"
#include "internal/group_cases.inl"
#include "internal/group_union.inl"

#include "internal/mirror.inl"
#include "internal/type_id.inl"
#include "internal/case_info.inl"
