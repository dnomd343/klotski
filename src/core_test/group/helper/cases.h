#pragma once

#include "group/group.h"

// TODO: move to `.cc` file
using klotski::cases::AllCases;
using klotski::codec::CommonCode;

// ----------------------------------------------------------------------------------------- //

// TODO: using global test helper

/// Get the type_id upper limit.
uint32_t group_union_num();

/// Get the group_id upper limit.
uint32_t group_num(uint32_t type_id);

/// Get cases contained in the specified type_id.
const std::vector<CommonCode>& group_union_cases(uint32_t type_id);

/// Get cases contained in the specified type_id and group_id.
const std::vector<CommonCode>& group_cases(uint32_t type_id, uint32_t group_id);

// ----------------------------------------------------------------------------------------- //
