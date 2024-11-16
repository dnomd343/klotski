#include "rust_ffi/include/short_code.h"

#include <short_code/short_code.h>

using klotski::codec::ShortCode;

using klotski::ffi::RsShortCode;
using klotski::ffi::RsCommonCode;

bool klotski::ffi::short_code_check(const uint32_t val) {
    return ShortCode::check(val);
}

uint32_t klotski::ffi::short_code_from_str(const rust::Str s) {
    const std::string_view sv {s.data(), s.length()};
    if (const auto ret = ShortCode::from_string(sv); ret.has_value()) {
        return ret.value().unwrap();
    }
    return 0xFFFFFFFF; // return invalid value for now
}

rust::String RsShortCode::to_string() const noexcept {
    return ShortCode::unsafe_create(code).to_string();
}

RsCommonCode RsShortCode::to_common_code() const noexcept {
    return {ShortCode::unsafe_create(code).to_common_code().unwrap()};
}

void klotski::ffi::short_code_speed_up(const bool fast_mode) {
    ShortCode::speed_up(fast_mode);
}
