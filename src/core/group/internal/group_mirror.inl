#pragma once

namespace klotski::group {

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

} // namespace klotski::group
