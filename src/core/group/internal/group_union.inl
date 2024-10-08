#pragma once

#include <ranges>

#include "constant/group_union.h"

namespace klotski::cases {

// ----------------------------------------------------------------------------------------- //

constexpr uint32_t GroupUnion::unwrap() const {
	return type_id_;
}

constexpr GroupUnion GroupUnion::unsafe_create(const uint32_t type_id) {
	return std::bit_cast<GroupUnion>(type_id);
}

constexpr std::optional<GroupUnion> GroupUnion::create(const uint32_t type_id) {
	if (type_id < TYPE_ID_LIMIT) {
		return unsafe_create(type_id);
	}
	return std::nullopt;
}

// ----------------------------------------------------------------------------------------- //

constexpr uint32_t GroupUnion::size() const {
	return GROUP_UNION_SIZE[type_id_];
}

constexpr uint32_t GroupUnion::group_num() const {
	return GROUP_NUM[type_id_];
}

constexpr uint32_t GroupUnion::max_group_size() const {
	return MAX_GROUP_SIZE[type_id_];
}

inline std::vector<Group> GroupUnion::groups() const {
	auto build = [this](const uint32_t group_id) {
		return Group::unsafe_create(type_id_, group_id);
	};
	return std::views::iota(0U, group_num())
		| std::views::transform(build)
		| std::ranges::to<std::vector>();
}

inline std::optional<Group> GroupUnion::group(const uint32_t group_id) const {
	if (group_id < group_num()) {
		return Group::unsafe_create(type_id_, group_id);
	}
	return std::nullopt;
}

// ----------------------------------------------------------------------------------------- //

inline GroupUnion GroupUnion::from_raw_code(const codec::RawCode raw_code) {
	return unsafe_create(type_id(raw_code));
}

inline GroupUnion GroupUnion::from_short_code(const codec::ShortCode short_code) {
	return from_common_code(short_code.to_common_code());
}

inline GroupUnion GroupUnion::from_common_code(const codec::CommonCode common_code) {
	return unsafe_create(type_id(common_code));
}

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::cases
