#pragma once

#include <span>
#include <functional>

#include "raw_code/raw_code.h"
#include "all_cases/all_cases.h"
#include "short_code/short_code.h"
#include "common_code/common_code.h"

using klotski::codec::RawCode;
using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

/// Build all valid CommonCodes.
std::vector<uint64_t> all_common_codes();

// ----------------------------------------------------------------------------------------- //

/// Spawn all valid RawCodes in parallel.
void raw_code_parallel(std::function<void(std::span<RawCode>)> &&func);

/// Spawn all valid ShortCodes in parallel.
void short_code_parallel(std::function<void(std::span<ShortCode>)> &&func);

/// Spawn all valid CommonCodes in parallel.
void common_code_parallel(std::function<void(std::span<CommonCode>)> &&func);

#define CODE_PARALLEL(Type, type, impl)                    \
    type##_code_parallel([](std::span<Type##Code> codes) { \
        for (auto code : codes) {impl}                     \
    })

#define RAW_CODE_PARALLEL(impl) CODE_PARALLEL(Raw, raw, impl)
#define SHORT_CODE_PARALLEL(impl) CODE_PARALLEL(Short, short, impl)
#define COMMON_CODE_PARALLEL(impl) CODE_PARALLEL(Common, common, impl)

// ----------------------------------------------------------------------------------------- //
