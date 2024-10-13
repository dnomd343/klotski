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
// TODO: new interface

constexpr uint32_t GroupUnion::pattern_num() const {
    return PATTERN_NUM[type_id_];
}

inline std::vector<GroupPro> GroupUnion::groups_pro() const {
    std::vector<GroupPro> groups;
    groups.reserve(group_num());
    for (uint32_t pattern_id = 0; pattern_id < pattern_num(); ++pattern_id) {
        auto group = GroupPro::unsafe_create(type_id_, pattern_id, 0);
        groups.emplace_back(group);
        switch (group.mirror_type()) {
            case GroupPro::MirrorType::FullMirror:
                continue;
            case GroupPro::MirrorType::HorizontalMirror:
                groups.emplace_back(GroupPro::unsafe_create(type_id_, pattern_id, 2));
                break;
            case GroupPro::MirrorType::CentroMirror:
            case GroupPro::MirrorType::VerticalMirror:
                groups.emplace_back(GroupPro::unsafe_create(type_id_, pattern_id, 1));
                break;
            case GroupPro::MirrorType::NonMirror:
                groups.emplace_back(GroupPro::unsafe_create(type_id_, pattern_id, 1));
                groups.emplace_back(GroupPro::unsafe_create(type_id_, pattern_id, 2));
                groups.emplace_back(GroupPro::unsafe_create(type_id_, pattern_id, 3));
                break;
        }
    }
    return groups;
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
