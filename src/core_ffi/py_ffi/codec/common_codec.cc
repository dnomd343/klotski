#include <format>

#include "py_exps.h"
#include "py_codec.h"

using klotski::ffi::PyCodecExp;

using klotski::ffi::PyShortCode;
using klotski::ffi::PyCommonCode;

using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

// ----------------------------------------------------------------------------------------- //

uint64_t PyCommonCode::value() const {
    return code_.unwrap();
}

std::string PyCommonCode::string() const {
    return code_.to_string(true);
}

PyShortCode PyCommonCode::short_code() const {
    return std::bit_cast<PyShortCode>(code_.to_short_code());
}

// ----------------------------------------------------------------------------------------- //

bool PyCommonCode::check(const uint64_t code) {
    return CommonCode::check(code);
}

bool PyCommonCode::check(const std::string_view code) {
    return CommonCode::from_string(code).has_value();
}

// ----------------------------------------------------------------------------------------- //

std::string PyCommonCode::str(const PyCommonCode code) {
    return code.code_.to_string();
}

std::string PyCommonCode::repr(const PyCommonCode code) {
    return std::format("<klotski.CommonCode 0x{}>", str(code));
}

// ----------------------------------------------------------------------------------------- //

static CommonCode convert(const PyShortCode code) {
    return std::bit_cast<ShortCode>(code).to_common_code();
}

static CommonCode convert(const uint64_t code) {
    if (CommonCode::check(code)) {
        return CommonCode::unsafe_create(code);
    }
    throw PyCodecExp(std::format("invalid common code -> {}", code));
}

static CommonCode convert(const std::string_view code) {
    if (const auto str = CommonCode::from_string(code)) {
        return str.value();
    }
    throw PyCodecExp(std::format("invalid common code -> `{}`", code));
}

PyCommonCode::PyCommonCode(const uint64_t code) : code_(convert(code)) {}

PyCommonCode::PyCommonCode(const PyShortCode code) : code_(convert(code)) {}

PyCommonCode::PyCommonCode(const std::string_view code) : code_(convert(code)) {}

// ----------------------------------------------------------------------------------------- //
