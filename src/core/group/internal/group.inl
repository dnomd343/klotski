#pragma once

#include "constant/group.h"

namespace klotski::cases {

[[nodiscard]] constexpr uint32_t Group::type_id() const {
    return type_id_;
}

[[nodiscard]] constexpr uint32_t Group::pattern_id() const {
    return pattern_id_;
}

[[nodiscard]] constexpr auto Group::toward() const -> Toward {
    return toward_;
}

constexpr Group Group::unsafe_create(uint32_t type_id, uint32_t pattern_id, Toward toward) {
    return {type_id, pattern_id, toward};
}

constexpr std::optional<Group> Group::create(uint32_t type_id, uint32_t pattern_id, Toward toward) {
    if (type_id >= TYPE_ID_LIMIT) {
        return std::nullopt;
    }
    if (pattern_id >= GroupUnion::unsafe_create(type_id).pattern_num()) {
        return std::nullopt;
    }
    // TODO: toward check
    return unsafe_create(type_id, pattern_id, toward);
}

constexpr uint32_t Group::flat_id() const {
    return PATTERN_OFFSET[type_id_] + pattern_id_;
}

constexpr uint32_t Group::size() const {
    return (PATTERN_DATA[flat_id()] >> 3) & 0xFFFFF;
}

inline Group Group::from_short_code(codec::ShortCode short_code) {
    return from_common_code(short_code.to_common_code());
}

inline Group Group::from_common_code(codec::CommonCode common_code) {
    return from_raw_code(common_code.to_raw_code());
}

constexpr auto Group::mirror_type() const -> MirrorType {
    return static_cast<MirrorType>(PATTERN_DATA[flat_id()] & 0b111);
}

constexpr bool Group::is_vertical_mirror() const {
    switch (mirror_type()) {
        case MirrorType::Full:
            return true;
        case MirrorType::Horizontal:
            return false;
        case MirrorType::Centro:
            return false;
        case MirrorType::Vertical:
            return true;
        case MirrorType::Ordinary:
            return false;
    }
}

constexpr bool Group::is_horizontal_mirror() const {
    switch (mirror_type()) {
        case MirrorType::Full:
            return true;
        case MirrorType::Horizontal:
            return true;
        case MirrorType::Centro:
            return false;
        case MirrorType::Vertical:
            return false;
        case MirrorType::Ordinary:
            return false;
    }
}

constexpr Group Group::to_vertical_mirror() const {
    switch (mirror_type()) {
        case MirrorType::Full:
            return *this;
        case MirrorType::Horizontal:
            if (toward_ == Toward::A) {
                return Group::unsafe_create(type_id_, pattern_id_, Toward::C);
            }
            return Group::unsafe_create(type_id_, pattern_id_, Toward::A);
        case MirrorType::Centro:
            if (toward_ == Toward::A) {
                return Group::unsafe_create(type_id_, pattern_id_, Toward::B);
            }
            return Group::unsafe_create(type_id_, pattern_id_, Toward::A);
        case MirrorType::Vertical:
            return *this;
        case MirrorType::Ordinary:
            if (toward_ == Toward::A) {
                return Group::unsafe_create(type_id_, pattern_id_, Toward::C);
            } else if (toward_ == Toward::B) {
                return Group::unsafe_create(type_id_, pattern_id_, Toward::D);
            } else if (toward_ == Toward::C) {
                return Group::unsafe_create(type_id_, pattern_id_, Toward::A);
            }
            return Group::unsafe_create(type_id_, pattern_id_, Toward::B);
    }
}

constexpr Group Group::to_horizontal_mirror() const {
    switch (mirror_type()) {
        case MirrorType::Full:
        case MirrorType::Horizontal:
            return *this;
        case MirrorType::Centro:
        case MirrorType::Vertical:
            if (toward_ == Toward::A) {
                return Group::unsafe_create(type_id_, pattern_id_, Toward::B);
            }
            return Group::unsafe_create(type_id_, pattern_id_, Toward::A);
        case MirrorType::Ordinary:
            if (toward_ == Toward::A) {
                return Group::unsafe_create(type_id_, pattern_id_, Toward::B);
            } else if (toward_ == Toward::B) {
                return Group::unsafe_create(type_id_, pattern_id_, Toward::A);
            } else if (toward_ == Toward::C) {
                return Group::unsafe_create(type_id_, pattern_id_, Toward::D);
            }
            return Group::unsafe_create(type_id_, pattern_id_, Toward::C);
    }
}

} // namespace klotski::cases
