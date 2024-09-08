#pragma once

#include <span>
#include <functional>

#include "all_cases/all_cases.h"
#include "short_code/short_code.h"
#include "common_code/common_code.h"

using klotski::cases::AllCases;
using klotski::codec::ShortCode;
using klotski::codec::CommonCode;
using klotski::cases::ALL_CASES_NUM_;

/// Build all valid CommonCodes.
std::vector<uint64_t> all_common_codes();

/// Spawn all valid klotski headers in parallel.
void head_parallel(std::function<void(uint64_t)> &&func);

void short_code_parallel(std::function<void(std::span<ShortCode>)> &&func);

void common_code_parallel(std::function<void(std::span<CommonCode>)> &&func);
