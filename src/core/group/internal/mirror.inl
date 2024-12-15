#pragma once

// TODO: class functions force inline

namespace klotski::group {

constexpr auto Group::mirror_type() const -> MirrorType {
    return static_cast<MirrorType>(PATTERN_DATA[flat_id()] & 0b111);
}

constexpr bool Group::is_vertical_mirror() const {
    switch (mirror_type()) {
        case MirrorType::Full:       return true;
        case MirrorType::Horizontal: return false;
        case MirrorType::Centro:     return false;
        case MirrorType::Vertical:   return true;
        case MirrorType::Ordinary:   return false;
    }
    std::unreachable();
}

constexpr bool Group::is_horizontal_mirror() const {
    switch (mirror_type()) {
        case MirrorType::Full:       return true;
        case MirrorType::Horizontal: return true;
        case MirrorType::Centro:     return false;
        case MirrorType::Vertical:   return false;
        case MirrorType::Ordinary:   return false;
    }
    std::unreachable();
}

constexpr Group Group::to_vertical_mirror() const {
    constexpr auto nil {Toward::A}; // placeholder
    constexpr auto towards = std::to_array({
        Toward::A,       nil,       nil,       nil, // MirrorType::Full
        Toward::C,       nil, Toward::A,       nil, // MirrorType::Horizontal
        Toward::B, Toward::A,       nil,       nil, // MirrorType::Centro
        Toward::A, Toward::B,       nil,       nil, // MirrorType::Vertical
        Toward::C, Toward::D, Toward::A, Toward::B, // MirrorType::Ordinary
    });
    const auto offset = static_cast<uint8_t>(mirror_type()) * 4;
    const auto toward = towards[offset + static_cast<uint8_t>(toward_)];
    return unsafe_create(type_id_, pattern_id_, toward);
}

constexpr Group Group::to_horizontal_mirror() const {
    constexpr auto nil {Toward::A}; // placeholder
    constexpr auto towards = std::to_array({
        Toward::A,       nil,       nil,       nil, // MirrorType::Full
        Toward::A,       nil, Toward::C,       nil, // MirrorType::Horizontal
        Toward::B, Toward::A,       nil,       nil, // MirrorType::Centro
        Toward::B, Toward::A,       nil,       nil, // MirrorType::Vertical
        Toward::B, Toward::A, Toward::D, Toward::C, // MirrorType::Ordinary
    });
    const auto offset = static_cast<uint8_t>(mirror_type()) * 4;
    const auto toward = towards[offset + static_cast<uint8_t>(toward_)];
    return unsafe_create(type_id_, pattern_id_, toward);
}

} // namespace klotski::group
