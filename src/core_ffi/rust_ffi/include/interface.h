#pragma once

#include "rust_ffi/src/bridge.rs.h"

namespace klotski::ffi {

bool short_code_check(uint32_t val);

bool layout_check(uint64_t val);

uint32_t short_code_from_str(rust::Str s);

uint64_t layout_from_str(rust::Str s);

void short_code_speed_up(bool fast_mode);

rust::String layout_to_str(uint64_t val);

rust::String layout_to_shorten_str(uint64_t val);

uint32_t layout_to_short_code(uint64_t val);

bool layout_is_horizontal_mirror(uint64_t val);

bool layout_is_vertical_mirror(uint64_t val);

uint64_t layout_to_horizontal_mirror(uint64_t val);

uint64_t layout_to_vertical_mirror(uint64_t val);

uint8_t layout_n_1x1(uint64_t val);

uint8_t layout_n_1x2(uint64_t val);

uint8_t layout_n_2x1(uint64_t val);

uint8_t layout_type_id(uint64_t val);

uint16_t layout_pattern_id(uint64_t val);

uint8_t layout_toward_char(uint64_t val);

uint32_t layout_case_id(uint64_t val);

rust::Vec<uint64_t> layout_next_cases(uint64_t val);

} // namespace klotski::ffi
