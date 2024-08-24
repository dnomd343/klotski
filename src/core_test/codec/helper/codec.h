#pragma once

#include <functional>

#include "all_cases/all_cases.h"

using klotski::cases::AllCases;
using klotski::cases::ALL_CASES_NUM_;

/// Build all valid CommonCodes.
std::vector<uint64_t> all_common_codes();

/// Spawn all valid klotski headers in parallel.
void head_parallel(std::function<void(uint64_t head)> &&func);
