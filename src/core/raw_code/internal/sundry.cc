#include "raw_code.h"
#include "common_code.h"

namespace klotski::codec {

// ----------------------------------------------------------------------------------------- //

RawCode::RawCode(const CommonCode common_code) {
    code_ = extract(common_code.unwrap());
}

// ----------------------------------------------------------------------------------------- //

CommonCode RawCode::to_common_code() const {
    return CommonCode::unsafe_create(compact(code_));
}

// ----------------------------------------------------------------------------------------- //

RawCode RawCode::from_common_code(const CommonCode common_code) {
    return common_code.to_raw_code();
}

std::optional<RawCode> RawCode::from_common_code(const uint64_t common_code) {
    auto convert = [](const CommonCode code) {
        return code.to_raw_code();
    };
    return CommonCode::create(common_code).transform(convert);
}

std::optional<RawCode> RawCode::from_common_code(const std::string &common_code) {
    auto convert = [](const CommonCode code) {
        return code.to_raw_code();
    };
    return CommonCode::from_string(common_code).transform(convert);
}

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::codec
