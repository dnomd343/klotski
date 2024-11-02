#pragma once

#include <ranges>

// TODO: include at group header
#include "constant/group.h"
#include "constant/group_union.h"

namespace klotski::group {

// ----------------------------------------------------------------------------------------- //

constexpr uint32_t GroupUnion::size() const {
    return GROUP_UNION_SIZE[type_id_];
}

constexpr uint32_t GroupUnion::group_num() const {
    return GROUP_NUM[type_id_];
}

constexpr uint32_t GroupUnion::pattern_num() const {
    return PATTERN_NUM[type_id_];
}

constexpr uint32_t GroupUnion::max_group_size() const {
    return MAX_GROUP_SIZE[type_id_];
}

// ----------------------------------------------------------------------------------------- //

constexpr uint_fast8_t GroupUnion::unwrap() const {
	return type_id_;
}

constexpr GroupUnion GroupUnion::unsafe_create(const uint_fast8_t type_id) {
	return std::bit_cast<GroupUnion>(type_id);
}

constexpr std::optional<GroupUnion> GroupUnion::create(const uint_fast8_t type_id) {
	if (type_id < TYPE_ID_LIMIT) {
		return unsafe_create(type_id);
	}
	return std::nullopt;
}

// ----------------------------------------------------------------------------------------- //

#ifndef KLSK_NDEBUG
inline std::ostream& operator<<(std::ostream &out, GroupUnion self) {
    out << self.type_id_;
    return out;
}
#endif

constexpr auto operator==(const GroupUnion &lhs, const GroupUnion &rhs) {
    return lhs.type_id_ == rhs.type_id_;
}

// ----------------------------------------------------------------------------------------- //

constexpr GroupUnion GroupUnion::from_raw_code(const codec::RawCode raw_code) {
    return unsafe_create(type_id(raw_code));
}

constexpr GroupUnion GroupUnion::from_short_code(const codec::ShortCode short_code) {
    return from_common_code(short_code.to_common_code());
}

constexpr GroupUnion GroupUnion::from_common_code(const codec::CommonCode common_code) {
    return unsafe_create(type_id(common_code));
}

// ----------------------------------------------------------------------------------------- //

//inline std::vector<Group> GroupUnion::groups() const {
//	auto build = [this](const uint32_t group_id) {
//		return Group::unsafe_create(type_id_, group_id);
//	};
//	return std::views::iota(0U, group_num())
//		| std::views::transform(build)
//		| std::ranges::to<std::vector>();
//}

constexpr std::vector<Group> GroupUnion::groups() const {

    // std::vector<Group> groups;
    // groups.reserve(group_num());
    // for (uint32_t pattern_id = 0; pattern_id < pattern_num(); ++pattern_id) {
    //     auto group = Group::unsafe_create(type_id_, pattern_id, Group::Toward::A);
    //     groups.emplace_back(group);
    //     switch (group.mirror_type()) {
    //         case Group::MirrorType::Full:
    //             continue;
    //         case Group::MirrorType::Horizontal:
    //             groups.emplace_back(Group::unsafe_create(type_id_, pattern_id, Group::Toward::C));
    //             break;
    //         case Group::MirrorType::Centro:
    //         case Group::MirrorType::Vertical:
    //             groups.emplace_back(Group::unsafe_create(type_id_, pattern_id, Group::Toward::B));
    //             break;
    //         case Group::MirrorType::Ordinary:
    //             groups.emplace_back(Group::unsafe_create(type_id_, pattern_id, Group::Toward::B));
    //             groups.emplace_back(Group::unsafe_create(type_id_, pattern_id, Group::Toward::C));
    //             groups.emplace_back(Group::unsafe_create(type_id_, pattern_id, Group::Toward::D));
    //             break;
    //     }
    // }
    // return groups;


    // auto build = [this](const uint32_t pattern_id) -> std::vector<Group> {
    //     auto flat_id = PATTERN_OFFSET[type_id_] + pattern_id;
    //     auto mirror_type = static_cast<Group::MirrorType>(PATTERN_DATA[flat_id] & 0b111);
    //
    //     switch (mirror_type) {
    //         case Group::MirrorType::Full:
    //             return {
    //                 Group::unsafe_create(type_id_, pattern_id, Group::Toward::A),
    //             };
    //         case Group::MirrorType::Horizontal:
    //             return {
    //                 Group::unsafe_create(type_id_, pattern_id, Group::Toward::A),
    //                 Group::unsafe_create(type_id_, pattern_id, Group::Toward::C),
    //             };
    //         case Group::MirrorType::Centro:
    //         case Group::MirrorType::Vertical:
    //             return {
    //                 Group::unsafe_create(type_id_, pattern_id, Group::Toward::A),
    //                 Group::unsafe_create(type_id_, pattern_id, Group::Toward::B),
    //             };
    //         case Group::MirrorType::Ordinary:
    //             return {
    //                 Group::unsafe_create(type_id_, pattern_id, Group::Toward::A),
    //                 Group::unsafe_create(type_id_, pattern_id, Group::Toward::B),
    //                 Group::unsafe_create(type_id_, pattern_id, Group::Toward::C),
    //                 Group::unsafe_create(type_id_, pattern_id, Group::Toward::D),
    //             };
    //     }
    // };
    //
    // return std::views::iota(0U, pattern_num())
    //      | std::views::transform(build)
    //      | std::views::join
    //      | std::ranges::to<std::vector>();

    // std::vector<Group> groups;
    // groups.reserve(group_num());

    auto begin = GROUP_OFFSET[type_id_];
    auto end = begin + group_num();

    auto build = [this](uint32_t kk) {
        auto raw = GROUP_DATA[kk];
        uint32_t pattern_id = (raw >> 2) & 0b1111111111;
        uint32_t toward = raw & 0b11;
        return Group::unsafe_create(type_id_, pattern_id, (Group::Toward)toward);
    };

    return std::views::iota(begin, end) | std::views::transform(build) | std::ranges::to<std::vector>();


    // auto build = [this](uint64_t raw) {
    //     uint32_t pattern_id = (raw >> 2) & 0b1111111111;
    //     uint32_t toward = raw & 0b11;
    //     return Group::unsafe_create(type_id_, pattern_id, (Group::Toward)toward);
    // };
    // return GROUP_DATA | std::views::drop(GROUP_OFFSET[type_id_]) | std::views::take(group_num()) | std::views::transform(build) | std::ranges::to<std::vector>();
    // return std::ranges::subrange(GROUP_DATA.begin() + begin, GROUP_DATA.begin() + begin + group_num()) | std::views::transform(build) | std::ranges::to<std::vector>();

    // for (auto i = begin; i < end; ++i) {
    //     auto raw = GROUP_DATA[i];
    //     uint32_t pattern_id = (raw >> 2) & 0b1111111111;
    //     uint32_t toward = raw & 0b11;
    //     auto group = Group::unsafe_create(type_id_, pattern_id, (Group::Toward)toward);
    //     groups.emplace_back(group);
    // }
    //
    // return groups;
}

constexpr std::optional<std::vector<Group>> GroupUnion::groups(const uint_fast16_t pattern_id) const {
    if (pattern_id >= pattern_num()) {
        return std::nullopt;
    }
    std::vector<Group> groups;
    auto group = Group::unsafe_create(type_id_, pattern_id, Group::Toward::A);
    groups.emplace_back(group);
    switch (group.mirror_type()) {
        case Group::MirrorType::Full:
            break;
        case Group::MirrorType::Horizontal:
            groups.emplace_back(Group::unsafe_create(type_id_, pattern_id, Group::Toward::C));
            break;
        case Group::MirrorType::Centro:
        case Group::MirrorType::Vertical:
            groups.emplace_back(Group::unsafe_create(type_id_, pattern_id, Group::Toward::B));
            break;
        case Group::MirrorType::Ordinary:
            groups.emplace_back(Group::unsafe_create(type_id_, pattern_id, Group::Toward::B));
            groups.emplace_back(Group::unsafe_create(type_id_, pattern_id, Group::Toward::C));
            groups.emplace_back(Group::unsafe_create(type_id_, pattern_id, Group::Toward::D));
            break;
    }
    return groups;
}

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::group
