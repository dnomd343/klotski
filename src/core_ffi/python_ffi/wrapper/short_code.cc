#include <format>

#include "exception.h"
#include "py_ffi/layout.h"
#include "py_ffi/short_code.h"

using namespace klotski::ffi;
using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

// ----------------------------------------------------------------------------------------- //

uint32_t PyShortCode::value() const noexcept {
    return code_.unwrap();
}

PyLayout PyShortCode::layout() const noexcept {
    return std::bit_cast<PyLayout>(code_.to_common_code());
}

// ----------------------------------------------------------------------------------------- //

bool PyShortCode::check(const uint32_t code) noexcept {
    return ShortCode::check(code);
}

bool PyShortCode::check(const std::string_view code) noexcept {
    return ShortCode::from_string(code).has_value();
}

// ----------------------------------------------------------------------------------------- //

std::string PyShortCode::str(const PyShortCode code) noexcept {
    return code.code_.to_string();
}

std::string PyShortCode::repr(const PyShortCode code) noexcept {
    return std::format("<klotski.ShortCode {} @{}>", code.value(), str(code));
}

// ----------------------------------------------------------------------------------------- //

static ShortCode convert(const PyLayout layout) noexcept {
    return std::bit_cast<CommonCode>(layout).to_short_code();
}

static ShortCode convert(const uint32_t code) {
    if (ShortCode::check(code)) {
        return ShortCode::unsafe_create(code);
    }
    throw PyExc_CodecError(std::format("invalid short code -> {}", code));
}

static ShortCode convert(const std::string_view code) {
    if (const auto str = ShortCode::from_string(code)) {
        return str.value();
    }
    throw PyExc_CodecError(std::format("invalid short code -> `{}`", code));
}

PyShortCode::PyShortCode(const uint32_t code) : code_(convert(code)) {}
PyShortCode::PyShortCode(const std::string_view code) : code_(convert(code)) {}
PyShortCode::PyShortCode(const PyLayout code) noexcept : code_(convert(code)) {}

// ----------------------------------------------------------------------------------------- //
