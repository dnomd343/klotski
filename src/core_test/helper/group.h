#pragma once

#include "common_code/common_code.h"

// /// Get the type_id upper limit.
// uint32_t group_union_num();
//
// /// Get the group_id upper limit.
// uint32_t group_num(uint32_t type_id);
//
// /// Get cases contained in the specified type_id.
// const std::vector<CommonCode>& group_union_cases(uint32_t type_id);
//
// /// Get cases contained in the specified type_id and group_id.
// const std::vector<CommonCode>& group_cases(uint32_t type_id, uint32_t group_id);

namespace helper {

using klotski::codec::CommonCode;

/// Get the type_id upper limit.
uint32_t group_union_num(); // TODO: remove it

/// Get cases contained in the specified type_id.
const std::vector<CommonCode>& group_union_cases(uint32_t type_id);

uint32_t group_union_pattern_num(uint32_t type_id);

uint32_t group_union_group_num(uint32_t type_id);

uint8_t pattern_mirror_type(uint32_t type_id, uint32_t pattern_id);

std::vector<uint8_t> pattern_toward_list(uint32_t type_id, uint32_t pattern_id);

const std::vector<CommonCode>& group_cases(uint32_t type_id, uint32_t pattern_id, uint8_t toward);

} // namespace helper
