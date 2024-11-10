#pragma once

#include "rust_ffi/src/common_code.rs.h"

namespace klotski::ffi {

RsCommonCode from_string(rust::Str s);

} // namespace klotski::ffi
