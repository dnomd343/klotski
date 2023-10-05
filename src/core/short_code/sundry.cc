#include "short_code.h"
#include "common_code.h"

using klotski::cases::AllCases;

namespace klotski {
namespace codec {

// ----------------------------------------------------------------------------------------- //

ShortCode::ShortCode(CommonCode common_code) noexcept {
    if (AllCases::instance().is_available()) {
        code_ = fast_encode(common_code.unwrap());
    } else {
        code_ = tiny_encode(common_code.unwrap());
    }
}

// ----------------------------------------------------------------------------------------- //

std::string ShortCode::to_string() const noexcept {
    return string_encode(code_);
}

CommonCode ShortCode::to_common_code() const noexcept {
    if (AllCases::instance().is_available()) {
        return CommonCode::unsafe_create(fast_decode(code_));
    }
    return CommonCode::unsafe_create(tiny_decode(code_));
}

// ----------------------------------------------------------------------------------------- //

std::optional<ShortCode> ShortCode::from_string(std::string &&short_code) noexcept {
    return ShortCode::from_string(short_code);
}

std::optional<ShortCode> ShortCode::from_string(const std::string &short_code) noexcept {
    return ShortCode::string_decode(short_code).transform([](auto code) {
        return ShortCode::unsafe_create(code);
    });
}

// ----------------------------------------------------------------------------------------- //

ShortCode ShortCode::from_common_code(CommonCode common_code) noexcept {
    return common_code.to_short_code();
}

std::optional<ShortCode> ShortCode::from_common_code(uint64_t common_code) noexcept {
    return CommonCode::create(common_code).transform([](auto common_code) {
        return common_code.to_short_code();
    });
}

std::optional<ShortCode> ShortCode::from_common_code(std::string &&common_code) noexcept {
    return CommonCode::from_string(std::move(common_code)).transform([](auto common_code) {
        return common_code.to_short_code();
    });
}

std::optional<ShortCode> ShortCode::from_common_code(const std::string &common_code) noexcept {
    return CommonCode::from_string(common_code).transform([](auto common_code) {
        return common_code.to_short_code();
    });
}

// ----------------------------------------------------------------------------------------- //

} // namespace codec
} // namespace klotski
