#include "raw_code.h"
#include "short_code.h"
#include "common_code.h"

// TODO: move to inline header

namespace klotski::codec {

// ----------------------------------------------------------------------------------------- //

CommonCode::CommonCode(RawCode raw_code) {
    code_ = raw_code.to_common_code().code_;
}

CommonCode::CommonCode(ShortCode short_code) {
    code_ = short_code.to_common_code().code_;
}

// ----------------------------------------------------------------------------------------- //

RawCode CommonCode::to_raw_code() const {
    return RawCode(*this);
}

ShortCode CommonCode::to_short_code() const {
    return ShortCode(*this);
}

// ----------------------------------------------------------------------------------------- //

CommonCode CommonCode::from_raw_code(RawCode raw_code) {
    return raw_code.to_common_code();
}

std::optional<CommonCode> CommonCode::from_raw_code(uint64_t raw_code) {
    return RawCode::create(raw_code).transform([](auto raw_code) {
        return raw_code.to_common_code();
    });
}

// ----------------------------------------------------------------------------------------- //

CommonCode CommonCode::from_short_code(ShortCode short_code) {
    return short_code.to_common_code();
}

std::optional<CommonCode> CommonCode::from_short_code(uint32_t short_code) {
    return ShortCode::create(short_code).transform([](auto short_code) {
        return short_code.to_common_code();
    });
}

std::optional<CommonCode> CommonCode::from_short_code(const std::string &short_code) {
    return ShortCode::from_string(short_code).transform([](auto short_code) {
        return short_code.to_common_code();
    });
}

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::codec
