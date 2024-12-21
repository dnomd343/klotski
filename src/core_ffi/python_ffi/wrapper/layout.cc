#include <format>

#include "exception.h"
#include "py_ffi/layout.h"

using namespace klotski::ffi;
using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

// ----------------------------------------------------------------------------------------- //

uint64_t PyLayout::value() const noexcept {
    return code_.unwrap();
}

std::string PyLayout::string(const bool shorten) const noexcept {
    return code_.to_string(shorten);
}

PyShortCode PyLayout::short_code() const noexcept {
    return std::bit_cast<PyShortCode>(code_.to_short_code());
}

// ----------------------------------------------------------------------------------------- //

bool PyLayout::check(const uint64_t code) noexcept {
    return CommonCode::check(code);
}

bool PyLayout::check(const std::string_view code) noexcept {
    return CommonCode::from_string(code).has_value();
}

// ----------------------------------------------------------------------------------------- //

[[nodiscard]] std::vector<PyLayout> PyLayout::next_cases() const noexcept {
    std::vector<PyLayout> cases;
    auto mover = mover::MaskMover([&cases](const codec::RawCode code, uint64_t) {
        cases.emplace_back(std::bit_cast<PyLayout>(code.to_common_code()));
    });
    mover.next_cases(code_.to_raw_code(), 0);
    return cases;
}

std::array<PyBlock, 20> PyLayout::dump_seq() const noexcept {
    std::array<PyBlock, 20> seq {};
    auto raw_code = code_.to_raw_code().unwrap();
    for (int i = 0; i < 20; ++i) {
        auto tmp = (raw_code >> (i * 3)) & 0b111;
        seq[i] = (PyBlock)(tmp);
    }
    return seq;
}

// ----------------------------------------------------------------------------------------- //

std::string PyLayout::str(const PyLayout code) noexcept {
    return code.code_.to_string();
}

std::string PyLayout::repr(const PyLayout code) noexcept {
    return std::format("<klotski.Layout 0x{}>", str(code));
}

// ----------------------------------------------------------------------------------------- //

static CommonCode convert(const PyShortCode code) noexcept {
    return std::bit_cast<ShortCode>(code).to_common_code();
}

static CommonCode convert(const uint64_t code) {
    if (CommonCode::check(code)) {
        return CommonCode::unsafe_create(code);
    }
    throw PyExc_CodecError(std::format("invalid code: {}", code));
}

static CommonCode convert(const std::string_view code) {
    if (const auto str = CommonCode::from_string(code)) {
        return str.value();
    }
    throw PyExc_CodecError(std::format("invalid code: `{}`", code));
}

PyLayout::PyLayout(const uint64_t code) : code_(convert(code)) {}
PyLayout::PyLayout(const std::string_view code) : code_(convert(code)) {}
PyLayout::PyLayout(const PyShortCode code) noexcept : code_(convert(code)) {}

// ----------------------------------------------------------------------------------------- //
