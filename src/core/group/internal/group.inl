#pragma once

#include <format>

#include "short_code/short_code.h"
#include "common_code/common_code.h"

namespace klotski::group {

// ----------------------------------------------------------------------------------------- //

constexpr uint32_t Group::flat_id() const {
    return PATTERN_OFFSET[type_id_] + pattern_id_;
}

constexpr uint32_t Group::size() const {
    return (PATTERN_DATA[flat_id()] >> 3) & 0xFFFFF;
}

// ----------------------------------------------------------------------------------------- //

constexpr auto Group::toward() const -> Toward {
    return toward_;
}

constexpr uint_fast8_t Group::type_id() const {
    return type_id_;
}

constexpr uint_fast16_t Group::pattern_id() const {
    return pattern_id_;
}

constexpr char Group::toward_char() const {
    // TODO: select chars from pre-build std::array
    switch (mirror_type()) {
        case MirrorType::Full:
            return 'x';
        case MirrorType::Horizontal:
            return (toward_ == Toward::A) ? 'n' : 'u';
        case MirrorType::Centro:
            return (toward_ == Toward::A) ? 's' : 'o';
        case MirrorType::Vertical:
            return (toward_ == Toward::A) ? 'p' : 'q';
        case MirrorType::Ordinary:
            if (toward_ == Toward::A) {
                return 'a';
            }
            if (toward_ == Toward::B) {
                return 'b';
            }
            if (toward_ == Toward::C) {
                return 'c';
            }
            if (toward_ == Toward::D) {
                return 'd';
            }
    }
    return '\0'; // TODO: never reach
}

// ----------------------------------------------------------------------------------------- //

inline Group Group::from_short_code(const codec::ShortCode short_code) {
    return from_common_code(short_code.to_common_code());
}

inline Group Group::from_common_code(const codec::CommonCode common_code) {
    return from_raw_code(common_code.to_raw_code());
}

// ----------------------------------------------------------------------------------------- //

#ifndef KLSK_NDEBUG
inline std::ostream& operator<<(std::ostream &out, const Group self) {
    out << self.to_string();
    return out;
}
#endif

KLSK_INLINE_H std::string Group::to_string() const {
    return std::format("{}-{}{}", type_id_, pattern_id_, toward_char());
}

constexpr auto operator==(const Group &lhs, const Group &rhs) {
    return lhs.toward_ == rhs.toward_
        && lhs.type_id_ == rhs.type_id_
        && lhs.pattern_id_ == rhs.pattern_id_;
}

// ----------------------------------------------------------------------------------------- //

constexpr Group::Group(const Toward toward,
                       const uint_fast8_t type_id,
                       const uint_least16_t pattern_id)
    : toward_(toward), type_id_(type_id), pattern_id_(pattern_id) {}

constexpr Group Group::unsafe_create(const uint_fast8_t type_id,
                                     const uint_least16_t pattern_id,
                                     const Toward toward) {
    return {toward, type_id, pattern_id};
}

constexpr std::optional<Group> Group::create(const uint_fast8_t type_id,
                                             const uint_least16_t pattern_id,
                                             const Toward toward) {
    if (type_id >= TYPE_ID_LIMIT || pattern_id >= PATTERN_NUM[type_id]) {
        return std::nullopt;
    }

    // TODO: perf check process
    if (toward == Toward::A) {
        return unsafe_create(type_id, pattern_id, toward);
    }

    auto mirror_type = PATTERN_DATA[PATTERN_OFFSET[type_id] + pattern_id] & 0b111;

    if (toward == Toward::B) {
        switch ((MirrorType)mirror_type) {
            case MirrorType::Full: return std::nullopt;
            case MirrorType::Horizontal: return std::nullopt;
            case MirrorType::Centro: return unsafe_create(type_id, pattern_id, toward);
            case MirrorType::Vertical: return unsafe_create(type_id, pattern_id, toward);
            case MirrorType::Ordinary: return unsafe_create(type_id, pattern_id, toward);
        }
    }

    if (toward == Toward::C) {
        switch ((MirrorType)mirror_type) {
            case MirrorType::Full: return std::nullopt;
            case MirrorType::Horizontal: return unsafe_create(type_id, pattern_id, toward);
            case MirrorType::Centro: return std::nullopt;
            case MirrorType::Vertical: return std::nullopt;
            case MirrorType::Ordinary: return unsafe_create(type_id, pattern_id, toward);
        }
    }

    if (toward == Toward::D) {
        switch ((MirrorType)mirror_type) {
            case MirrorType::Full: return std::nullopt;
            case MirrorType::Horizontal: return std::nullopt;
            case MirrorType::Centro: return std::nullopt;
            case MirrorType::Vertical: return std::nullopt;
            case MirrorType::Ordinary: return unsafe_create(type_id, pattern_id, toward);
        }
    }

    return std::nullopt; // TODO: never reach
}

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::group
