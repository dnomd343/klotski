#include "rust_ffi/include/common_code.h"

#include <common_code/common_code.h>

using klotski::ffi::RsCommonCode;

rust::String RsCommonCode::to_string() const noexcept {
    return codec::CommonCode::unsafe_create(code).to_string();
}

// TODO: it seems that cxx.rs not support `std::optional`
RsCommonCode klotski::ffi::from_string(rust::Str s) {
    std::string_view sv {s.data(), s.length()};
    return {codec::CommonCode::from_string(sv)->unwrap()}; // TODO: value check
}
