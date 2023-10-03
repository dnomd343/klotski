#include "raw_code.h"
#include "short_code.h"
#include "common_code.h"

namespace klotski {
namespace codec {

// ----------------------------------------------------------------------------------------- //

CommonCode::CommonCode(RawCode raw_code) noexcept {
    code_ = raw_code.to_common_code().code_;
}

CommonCode::CommonCode(ShortCode short_code) noexcept {
    code_ = short_code.to_common_code().code_;
}

// ----------------------------------------------------------------------------------------- //

RawCode CommonCode::to_raw_code() const noexcept {
    return RawCode(*this);
}

ShortCode CommonCode::to_short_code() const noexcept {
    return ShortCode(*this);
}

std::string CommonCode::to_string(bool shorten) const noexcept {
    if (!shorten) {
        return string_encode(code_); // with full length
    }
    return string_encode_shorten(code_); // without trailing zero
}

// ----------------------------------------------------------------------------------------- //

std::optional<CommonCode> CommonCode::from_string(std::string &&common_code) noexcept {
    return CommonCode::from_string(common_code);
}

std::optional<CommonCode> CommonCode::from_string(const std::string &common_code) noexcept {
    auto code = string_decode(common_code);
    if (!code.has_value()) {
        return std::nullopt; // invalid string
    }
    return CommonCode::unsafe_create(code.value());
}

// ----------------------------------------------------------------------------------------- //

CommonCode CommonCode::from_raw_code(RawCode raw_code) noexcept {
    return raw_code.to_common_code();
}

std::optional<CommonCode> CommonCode::from_raw_code(uint64_t raw_code) noexcept {
    auto code = RawCode::create(raw_code);
    if (!code.has_value()) {
        return std::nullopt; // invalid raw code
    }
    return code->to_common_code();
}

// ----------------------------------------------------------------------------------------- //

CommonCode CommonCode::from_short_code(ShortCode short_code) noexcept {
    return short_code.to_common_code();
}

std::optional<CommonCode> CommonCode::from_short_code(uint32_t short_code) noexcept {
    auto code = ShortCode::create(short_code);
    if (!code.has_value()) {
        return std::nullopt; // invalid short code
    }
    return code->to_common_code();
}

std::optional<CommonCode> CommonCode::from_short_code(std::string &&short_code) noexcept {
    auto code = ShortCode::from_string(std::move(short_code));
    if (!code.has_value()) {
        return std::nullopt; // invalid short code
    }
    return code->to_common_code();
}

std::optional<CommonCode> CommonCode::from_short_code(const std::string &short_code) noexcept {
    auto code = ShortCode::from_string(short_code);
    if (!code.has_value()) {
        return std::nullopt; // invalid short code
    }
    return code->to_common_code();
}

// ----------------------------------------------------------------------------------------- //

} // namespace codec
} // namespace klotski
