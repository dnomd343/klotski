#include "raw_code.h"
#include "common_code.h"

namespace klotski {
namespace codec {

// ----------------------------------------------------------------------------------------- //

RawCode::RawCode(CommonCode common_code) noexcept {
    code_ = RawCode::extract(common_code.unwrap());
}

// ----------------------------------------------------------------------------------------- //

CommonCode RawCode::to_common_code() const noexcept {
    return CommonCode::unsafe_create(RawCode::compact(code_));
}

// ----------------------------------------------------------------------------------------- //

RawCode RawCode::from_common_code(CommonCode common_code) noexcept {
    return common_code.to_raw_code();
}

std::optional<RawCode> RawCode::from_common_code(uint64_t common_code) noexcept {
    auto code = CommonCode::create(common_code);
    if (!code.has_value()) {
        return std::nullopt; // invalid common code
    }
    return code->to_raw_code();
}

std::optional<RawCode> RawCode::from_common_code(std::string &&common_code) noexcept {
    auto code = CommonCode::from_string(std::move(common_code));
    if (!code.has_value()) {
        return std::nullopt; // invalid common code
    }
    return code->to_raw_code();
}

std::optional<RawCode> RawCode::from_common_code(const std::string &common_code) noexcept {
    auto code = CommonCode::from_string(common_code);
    if (!code.has_value()) {
        return std::nullopt; // invalid common code
    }
    return code->to_raw_code();
}

// ----------------------------------------------------------------------------------------- //

} // namespace codec
} // namespace klotski
