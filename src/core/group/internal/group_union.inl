#pragma once

#include <ranges>

#include "raw_code/raw_code.h"
#include "short_code/short_code.h"
#include "common_code/common_code.h"

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
    out << std::format("{}", self.type_id_);
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

inline GroupUnion GroupUnion::from_short_code(const codec::ShortCode short_code) {
    return from_common_code(short_code.to_common_code());
}

constexpr GroupUnion GroupUnion::from_common_code(const codec::CommonCode common_code) {
    return unsafe_create(type_id(common_code));
}

// ----------------------------------------------------------------------------------------- //

// TODO: benchmark of force-inline
inline auto GroupUnion::groups() const -> Groups {
    auto build = [this](const size_t offset) {
        const auto raw = GROUP_DATA[offset];
        const uint_fast16_t pattern_id = (raw >> 2) & 0x3FF;
        const auto toward = static_cast<Group::Toward>(raw & 0b11);
        return Group::unsafe_create(type_id_, pattern_id, toward);
    };
    const auto offset = GROUP_OFFSET[type_id_];
    return std::views::iota(offset, offset + group_num())
         | std::views::transform(build)
         | std::ranges::to<std::vector>();
}

// TODO: benchmark of force-inline
inline auto GroupUnion::groups(const uint_least16_t pattern_id) const -> std::optional<Groups> {
    if (pattern_id >= pattern_num()) {
        return std::nullopt;
    }

    const auto flat_id = PATTERN_OFFSET[type_id_] + pattern_id;
    switch (static_cast<Group::MirrorType>(PATTERN_DATA[flat_id] & 0b111)) {
        case Group::MirrorType::Full:
            return Groups {
                Group::unsafe_create(type_id_, pattern_id, Group::Toward::A),
            };
        case Group::MirrorType::Horizontal:
            return Groups {
                Group::unsafe_create(type_id_, pattern_id, Group::Toward::A),
                Group::unsafe_create(type_id_, pattern_id, Group::Toward::C),
            };
        case Group::MirrorType::Centro:
        case Group::MirrorType::Vertical:
            return Groups {
                Group::unsafe_create(type_id_, pattern_id, Group::Toward::A),
                Group::unsafe_create(type_id_, pattern_id, Group::Toward::B),
            };
        case Group::MirrorType::Ordinary:
            return Groups {
                Group::unsafe_create(type_id_, pattern_id, Group::Toward::A),
                Group::unsafe_create(type_id_, pattern_id, Group::Toward::B),
                Group::unsafe_create(type_id_, pattern_id, Group::Toward::C),
                Group::unsafe_create(type_id_, pattern_id, Group::Toward::D),
            };
    }
    KLSK_UNREACHABLE;
}

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::group
