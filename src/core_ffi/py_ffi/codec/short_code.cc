#include <format>

#include "py_exps.h"
#include "py_codec.h"

using klotski::ffi::PyCodecExp;

using klotski::ffi::PyShortCode;
using klotski::ffi::PyCommonCode;

using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

// ----------------------------------------------------------------------------------------- //

uint32_t PyShortCode::value() const {
    return code_.unwrap();
}

PyCommonCode PyShortCode::common_code() const {
    return std::bit_cast<PyCommonCode>(code_.to_common_code());
}

// ----------------------------------------------------------------------------------------- //

bool PyShortCode::check(const uint32_t code) {
    return ShortCode::check(code);
}

bool PyShortCode::check(const std::string_view code) {
    // TODO: using `std::string_view` in from_string
    return ShortCode::from_string(std::string {code}).has_value();
}

void PyShortCode::speed_up(const bool fast_mode) {
    ShortCode::speed_up(fast_mode);
}

// ----------------------------------------------------------------------------------------- //

std::string PyShortCode::str(const PyShortCode code) {
    return std::bit_cast<ShortCode>(code).to_string();
}

std::string PyShortCode::repr(const PyShortCode code) {
    const auto str = code.code_.to_string();
    return std::format("<klotski.ShortCode {} @{}>", code.value(), str);
}

// ----------------------------------------------------------------------------------------- //

static ShortCode convert(const PyCommonCode code) {
    return std::bit_cast<CommonCode>(code).to_short_code();
}

static ShortCode convert(const uint32_t code) {
    if (ShortCode::check(code)) {
        return ShortCode::unsafe_create(code);
    }
    throw PyCodecExp(std::format("invalid short code -> {}", code));
}

static ShortCode convert(const std::string_view code) {
    // TODO: using `std::string_view` in from_string
    if (const auto str = ShortCode::from_string(std::string {code})) {
        return str.value();
    }
    throw PyCodecExp(std::format("invalid short code -> `{}`", code));
}

PyShortCode::PyShortCode(const uint32_t code) : code_(convert(code)) {}

PyShortCode::PyShortCode(const PyCommonCode code) : code_(convert(code)) {}

PyShortCode::PyShortCode(const std::string_view code) : code_(convert(code)) {}

// ----------------------------------------------------------------------------------------- //
