#pragma once

#include "rust_ffi/src/common_code.rs.h"

namespace klotski::ffi {

bool common_code_check(uint64_t val);

uint64_t common_code_from_str(rust::Str s);

} // namespace klotski::ffi
