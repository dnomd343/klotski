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

namespace klotski::cases {

constexpr uint32_t TYPE_ID_LIMIT = 203;
constexpr uint32_t ALL_GROUP_NUM = 25422;

typedef std::vector<codec::RawCode> RawCodes;
typedef std::vector<codec::CommonCode> CommonCodes;

class Group;

// TODO: add constexpr
class GroupUnion {
public:
	GroupUnion() = delete;

	// ------------------------------------------------------------------------------------- //

	/// Get the original type id.
	[[nodiscard]] constexpr uint32_t unwrap() const;

	/// Create GroupUnion without any check.
	static constexpr GroupUnion unsafe_create(uint32_t type_id);

	/// Create GroupUnion with validity check.
	static constexpr std::optional<GroupUnion> create(uint32_t type_id);

	// ------------------------------------------------------------------------------------- //

	/// Get the number of cases contained.
	[[nodiscard]] constexpr uint32_t size() const;

	/// Get the number of groups contained.
	[[nodiscard]] constexpr uint32_t group_num() const;

	/// Get the upper limit of the group size.
	[[nodiscard]] constexpr uint32_t max_group_size() const;

	// ------------------------------------------------------------------------------------- //

	/// Get all cases under the current type id.
	[[nodiscard]] RangesUnion cases() const;

	/// Get all groups under the current type id.
	[[nodiscard]] std::vector<Group> groups() const;

	/// Get the group instance with the specified group id.
	[[nodiscard]] std::optional<Group> group(uint32_t group_id) const;

	// ------------------------------------------------------------------------------------- //

	/// Create GroupUnion from RawCode.
	static GroupUnion from_raw_code(codec::RawCode raw_code);

	/// Create GroupUnion from ShortCode.
	static GroupUnion from_short_code(codec::ShortCode short_code);

	/// Create GroupUnion from CommonCode.
	static GroupUnion from_common_code(codec::CommonCode common_code);

	// ------------------------------------------------------------------------------------- //

private:
	uint32_t type_id_;

	// ------------------------------------------------------------------------------------- //

	/// Get the type id of RawCode.
	static KLSK_INLINE uint32_t type_id(codec::RawCode raw_code);

	/// Get the type id of CommonCode.
	static KLSK_INLINE uint32_t type_id(codec::CommonCode common_code);

	// ------------------------------------------------------------------------------------- //
};

class Group {
public:
	Group() = delete;

	// ------------------------------------------------------------------------------------- //

	/// Get the original type id.
	[[nodiscard]] uint32_t type_id() const;

	/// Get the original group id.
	[[nodiscard]] uint32_t group_id() const;

	/// Create Group without any check.
	static Group unsafe_create(uint32_t type_id, uint32_t group_id);

	/// Create Group with validity check.
	static std::optional<Group> create(uint32_t type_id, uint32_t group_id);

	// ------------------------------------------------------------------------------------- //

	/// Get the number of cases contained.
	[[nodiscard]] uint32_t size() const;

	/// Get all cases under the current group.
	[[nodiscard]] RangesUnion cases() const;

	// ------------------------------------------------------------------------------------- //

	static Group from_raw_code(codec::RawCode raw_code);

	static Group from_short_code(codec::ShortCode short_code);

	static Group from_common_code(codec::CommonCode common_code);

	// ------------------------------------------------------------------------------------- //

private:
	uint32_t type_id_;
	uint32_t group_id_;

	[[nodiscard]] uint32_t flat_id() const;

public:
	static std::vector<codec::RawCode> extend(codec::RawCode raw_code, uint32_t reserve = 0);
};

class GroupCase {
public:
	struct info_t {
		uint16_t type_id;
		uint16_t group_id;
		uint32_t group_index;
	};

	// TODO: mark as instance.

	/// Build group cases accelerated index.
	static void speed_up();

	/// Get the CommonCode using the group info.
	static codec::CommonCode parse(const info_t &info);

	/// Get group info according to specified case.
	static info_t encode(const codec::RawCode &raw_code);
	static info_t encode(const codec::CommonCode &common_code);

private:
	static bool available_;
	static std::mutex building_;

	static codec::CommonCode fast_decode(const info_t &info);
	static info_t fast_encode(const codec::CommonCode &common_code);
};

} // namespace klotski::cases

#include "internal/group_union.inl"
#include "internal/group.inl"
