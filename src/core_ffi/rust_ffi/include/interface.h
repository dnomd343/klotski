#pragma once

#include "rust_ffi/src/bridge.rs.h"

namespace klotski::ffi {

bool short_code_check(uint32_t val);

bool common_code_check(uint64_t val);

uint32_t short_code_from_str(rust::Str s);

uint64_t common_code_from_str(rust::Str s);

void short_code_speed_up(bool fast_mode);

} // namespace klotski::ffi
