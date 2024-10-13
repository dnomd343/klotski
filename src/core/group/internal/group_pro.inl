#pragma once

#include "constant/group_pro.h"

namespace klotski::cases {

constexpr uint32_t GroupPro::flat_id() const {
    return PATTERN_OFFSET[type_id_] + pattern_id_;
}

constexpr uint32_t GroupPro::size() const {
    return GROUP_PRO_SIZE[flat_id()];
}

inline GroupPro GroupPro::from_common_code(codec::CommonCode common_code) {
    return from_raw_code(common_code.to_raw_code());
}

constexpr GroupPro::MirrorType GroupPro::mirror_type() const {
    switch (GROUP_PRO_TYPE[flat_id()]) {
        case 0:
            return MirrorType::FullMirror;
        case 1:
            return MirrorType::HorizontalMirror;
        case 2:
            return MirrorType::CentroMirror;
        case 3:
            return MirrorType::VerticalMirror;
        case 4:
            return MirrorType::NonMirror;
    }
}

constexpr bool GroupPro::is_vertical_mirror() const {
    switch (mirror_type()) {
        case MirrorType::FullMirror:
            return true;
        case MirrorType::HorizontalMirror:
            return false;
        case MirrorType::CentroMirror:
            return false;
        case MirrorType::VerticalMirror:
            return true;
        case MirrorType::NonMirror:
            return false;
    }
}

constexpr bool GroupPro::is_horizontal_mirror() const {
    switch (mirror_type()) {
        case MirrorType::FullMirror:
            return true;
        case MirrorType::HorizontalMirror:
            return true;
        case MirrorType::CentroMirror:
            return false;
        case MirrorType::VerticalMirror:
            return false;
        case MirrorType::NonMirror:
            return false;
    }
}

constexpr GroupPro GroupPro::to_vertical_mirror() const {
    switch (mirror_type()) {
        case MirrorType::FullMirror:
            return *this;
        case MirrorType::HorizontalMirror:
            if (mirror_toward_ == 0) {
                return GroupPro::unsafe_create(type_id_, pattern_id_, 2);
            }
            return GroupPro::unsafe_create(type_id_, pattern_id_, 0);
        case MirrorType::CentroMirror:
            if (mirror_toward_ == 0) {
                return GroupPro::unsafe_create(type_id_, pattern_id_, 1);
            }
            return GroupPro::unsafe_create(type_id_, pattern_id_, 0);
        case MirrorType::VerticalMirror:
            return *this;
        case MirrorType::NonMirror:
            if (mirror_toward_ == 0) {
                return GroupPro::unsafe_create(type_id_, pattern_id_, 2);
            } else if (mirror_toward_ == 1) {
                return GroupPro::unsafe_create(type_id_, pattern_id_, 3);
            } else if (mirror_toward_ == 2) {
                return GroupPro::unsafe_create(type_id_, pattern_id_, 0);
            }
            return GroupPro::unsafe_create(type_id_, pattern_id_, 1);
    }
}

constexpr GroupPro GroupPro::to_horizontal_mirror() const {
    switch (mirror_type()) {
        case MirrorType::FullMirror:
        case MirrorType::HorizontalMirror:
            return *this;
        case MirrorType::CentroMirror:
        case MirrorType::VerticalMirror:
            if (mirror_toward_ == 0) {
                return GroupPro::unsafe_create(type_id_, pattern_id_, 1);
            }
            return GroupPro::unsafe_create(type_id_, pattern_id_, 0);
        case MirrorType::NonMirror:
            if (mirror_toward_ == 0) {
                return GroupPro::unsafe_create(type_id_, pattern_id_, 1);
            } else if (mirror_toward_ == 1) {
                return GroupPro::unsafe_create(type_id_, pattern_id_, 0);
            } else if (mirror_toward_ == 2) {
                return GroupPro::unsafe_create(type_id_, pattern_id_, 3);
            }
            return GroupPro::unsafe_create(type_id_, pattern_id_, 2);
    }
}

} // namespace klotski::cases
