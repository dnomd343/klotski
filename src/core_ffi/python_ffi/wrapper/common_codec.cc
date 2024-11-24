#include <format>

#include "exception.h"
#include "py_ffi/common_code.h"

using namespace klotski::ffi;

// ----------------------------------------------------------------------------------------- //

uint64_t PyCommonCode::value() const noexcept {
    return code_.unwrap();
}

std::string PyCommonCode::string(const bool shorten) const noexcept {
    return code_.to_string(shorten);
}

PyShortCode PyCommonCode::short_code() const noexcept {
    return std::bit_cast<PyShortCode>(code_.to_short_code());
}

// ----------------------------------------------------------------------------------------- //

bool PyCommonCode::check(const uint64_t code) noexcept {
    return CommonCode::check(code);
}

bool PyCommonCode::check(const std::string_view code) noexcept {
    return CommonCode::from_string(code).has_value();
}

// ----------------------------------------------------------------------------------------- //

std::string PyCommonCode::str(const PyCommonCode code) noexcept {
    return code.code_.to_string();
}

std::string PyCommonCode::repr(const PyCommonCode code) noexcept {
    return std::format("<klotski.CommonCode 0x{}>", str(code));
}

// ----------------------------------------------------------------------------------------- //

static CommonCode convert(const PyShortCode code) noexcept {
    return std::bit_cast<ShortCode>(code).to_common_code();
}

static CommonCode convert(const uint64_t code) {
    if (CommonCode::check(code)) {
        return CommonCode::unsafe_create(code);
    }
    throw PyExc_CodecError(std::format("invalid common code -> {}", code));
}

static CommonCode convert(const std::string_view code) {
    if (const auto str = CommonCode::from_string(code)) {
        return str.value();
    }
    throw PyExc_CodecError(std::format("invalid common code -> `{}`", code));
}

PyCommonCode::PyCommonCode(const uint64_t code) : code_(convert(code)) {}
PyCommonCode::PyCommonCode(const std::string_view code) : code_(convert(code)) {}
PyCommonCode::PyCommonCode(const PyShortCode code) noexcept : code_(convert(code)) {}

// ----------------------------------------------------------------------------------------- //
