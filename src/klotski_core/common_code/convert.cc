#include "common_code.h"

using klotski::RawCode;
using klotski::ShortCode;
using klotski::CommonCode;

using klotski::RawCodes;
using klotski::ShortCode;
using klotski::CommonCodes;

/// -------------------------- CommonCode to RawCode --------------------------

RawCode CommonCode::to_raw_code() const noexcept {
    return RawCode(*this); // convert to raw code
}

/// ------------------------- CommonCode to ShortCode -------------------------

ShortCode CommonCode::to_short_code() const noexcept {
    return ShortCode(*this); // convert to short code
}

/// -------------------------- RawCode to CommonCode --------------------------

CommonCode CommonCode::from_raw_code(uint64_t raw_code) {
    return RawCode(raw_code).to_common_code();
}

CommonCode CommonCode::from_raw_code(RawCode &&raw_code) noexcept {
    return raw_code.to_common_code();
}

CommonCode CommonCode::from_raw_code(const RawCode &raw_code) noexcept {
    return raw_code.to_common_code();
}

CommonCode::CommonCode(RawCode &&raw_code) noexcept {
    code_ = raw_code.to_common_code().code_; // convert from raw code
}

CommonCode::CommonCode(const RawCode &raw_code) noexcept {
    code_ = raw_code.to_common_code().code_; // convert from raw code
}

/// ------------------------- ShortCode to CommonCode -------------------------

CommonCode::CommonCode(ShortCode &&short_code) noexcept {
    code_ = short_code.to_common_code().code_; // convert from short code
}

CommonCode::CommonCode(const ShortCode &short_code) noexcept {
    code_ = short_code.to_common_code().code_; // convert from short code
}

CommonCode CommonCode::from_short_code(uint32_t short_code) {
    return ShortCode(short_code).to_common_code();
}

CommonCode CommonCode::from_short_code(ShortCode &&short_code) noexcept {
    return short_code.to_common_code();
}

CommonCode CommonCode::from_short_code(std::string &&short_code) {
    return ShortCode(std::forward<std::string>(short_code)).to_common_code();
}

CommonCode CommonCode::from_short_code(const ShortCode &short_code) noexcept {
    return short_code.to_common_code();
}

CommonCode CommonCode::from_short_code(const std::string &short_code) {
    return ShortCode(short_code).to_common_code();
}

/// ---------------------------- Batch conversions ----------------------------

CommonCodes CommonCode::convert(const RawCodes &raw_codes) noexcept {
    return {raw_codes.begin(), raw_codes.end()};
}

CommonCodes CommonCode::convert(const ShortCodes &short_codes) noexcept {
    return {short_codes.begin(), short_codes.end()};
}
