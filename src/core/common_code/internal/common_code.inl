#pragma once

#include "raw_code/raw_code.h"
#include "short_code/short_code.h"

namespace klotski::codec {

// ----------------------------------------------------------------------------------------- //

constexpr CommonCode::CommonCode(const RawCode raw_code) {
    code_ = raw_code.to_common_code().code_;
}

inline CommonCode::CommonCode(const ShortCode short_code) {
    code_ = short_code.to_common_code().code_;
}

constexpr CommonCode CommonCode::unsafe_create(const uint64_t common_code) {
    return std::bit_cast<CommonCode>(common_code); // init directly
}

constexpr std::optional<CommonCode> CommonCode::create(const uint64_t common_code) {
    if (!check(common_code)) {
        return std::nullopt; // invalid common code
    }
    return unsafe_create(common_code);
}

// ----------------------------------------------------------------------------------------- //

constexpr CommonCode::operator uint64_t() const {
    return code_;
}

#ifndef KLSK_NDEBUG
inline std::ostream& operator<<(std::ostream &out, const CommonCode self) {
    out << CommonCode::string_encode(self.code_);
    return out;
}
#endif

// ----------------------------------------------------------------------------------------- //

constexpr uint64_t CommonCode::unwrap() const {
    return code_;
}

constexpr RawCode CommonCode::to_raw_code() const {
    return RawCode(*this);
}

inline ShortCode CommonCode::to_short_code() const {
    return ShortCode(*this);
}

// ----------------------------------------------------------------------------------------- //

inline std::string CommonCode::to_string(const bool shorten) const {
    if (!shorten) {
        return string_encode(code_); // with full length
    }
    return string_encode_shorten(code_); // without trailing zero
}

inline std::optional<CommonCode> CommonCode::from_string(const std::string_view common_code) {
    return string_decode(common_code).transform(unsafe_create);
}

// ----------------------------------------------------------------------------------------- //

constexpr CommonCode CommonCode::from_raw_code(const RawCode raw_code) {
    return raw_code.to_common_code();
}

constexpr std::optional<CommonCode> CommonCode::from_raw_code(const uint64_t raw_code) {
    const auto convert = [](const RawCode code) {
        return code.to_common_code();
    };
    return RawCode::create(raw_code).transform(convert);
}

// ----------------------------------------------------------------------------------------- //

inline CommonCode CommonCode::from_short_code(const ShortCode short_code) {
    return short_code.to_common_code();
}

inline std::optional<CommonCode> CommonCode::from_short_code(const uint32_t short_code) {
    const auto convert = [](const ShortCode code) {
        return code.to_common_code();
    };
    return ShortCode::create(short_code).transform(convert);
}

inline std::optional<CommonCode> CommonCode::from_short_code(const std::string_view short_code) {
    const auto convert = [](const ShortCode code) {
        return code.to_common_code();
    };
    return ShortCode::from_string(short_code).transform(convert);
}

// ----------------------------------------------------------------------------------------- //

constexpr bool CommonCode::is_vertical_mirror() const {
    return false;
}

constexpr bool CommonCode::is_horizontal_mirror() const {
    return check_mirror(code_);
}

constexpr CommonCode CommonCode::to_vertical_mirror() const {
    return unsafe_create(get_vertical_mirror(code_));
}

constexpr CommonCode CommonCode::to_horizontal_mirror() const {
    return unsafe_create(get_horizontal_mirror(code_));
}

// ----------------------------------------------------------------------------------------- //

constexpr auto operator==(const CommonCode &lhs, const uint64_t rhs) {
    return lhs.code_ == rhs;
}

constexpr auto operator<=>(const CommonCode &lhs, const uint64_t rhs) {
    return lhs.code_ <=> rhs;
}

constexpr auto operator==(const CommonCode &lhs, const CommonCode &rhs) {
    return lhs.code_ == rhs.code_;
}

constexpr auto operator<=>(const CommonCode &lhs, const CommonCode &rhs) {
    return lhs.code_ <=> rhs.code_;
}

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::codec
