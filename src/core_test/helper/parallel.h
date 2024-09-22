#pragma once

#include <span>

#include "raw_code/raw_code.h"
#include "short_code/short_code.h"
#include "common_code/common_code.h"

using klotski::codec::RawCode;
using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

using klotski::cases::GroupUnion;

namespace helper {

// ----------------------------------------------------------------------------------------- //

void type_id_parallel(std::function<void(uint32_t type_id)> &&func);

void group_union_parallel(std::function<void(GroupUnion group_union)> &&func);

#define TYPE_ID_PARALLEL(impl) \
    helper::type_id_parallel([](uint32_t type_id) {impl})

#define GROUP_UNION_PARALLEL(impl) \
    helper::group_union_parallel([](GroupUnion group_union) {impl})

// ----------------------------------------------------------------------------------------- //

/// Spawn all valid RawCodes in parallel.
void raw_code_parallel(std::function<void(std::span<RawCode>)> &&func);

/// Spawn all valid ShortCodes in parallel.
void short_code_parallel(std::function<void(std::span<ShortCode>)> &&func);

/// Spawn all valid CommonCodes in parallel.
void common_code_parallel(std::function<void(std::span<CommonCode>)> &&func);

#define CODE_PARALLEL(Type, type, impl)                            \
    helper::type##_code_parallel([](std::span<Type##Code> codes) { \
        for (auto code : codes) {impl}                             \
    })

#define RAW_CODE_PARALLEL(impl) CODE_PARALLEL(Raw, raw, impl)
#define SHORT_CODE_PARALLEL(impl) CODE_PARALLEL(Short, short, impl)
#define COMMON_CODE_PARALLEL(impl) CODE_PARALLEL(Common, common, impl)

// ----------------------------------------------------------------------------------------- //

} // namespace helper
