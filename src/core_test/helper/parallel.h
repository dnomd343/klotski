#pragma once

#include <span>
#include <cstdint>
#include <functional>

#include "group/group.h"
#include "raw_code/raw_code.h"
#include "short_code/short_code.h"
#include "common_code/common_code.h"

using klotski::group::Group;
using klotski::group::GroupUnion;

using klotski::codec::RawCode;
using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

namespace helper {

// ----------------------------------------------------------------------------------------- //

void head_parallel(std::function<void(uint64_t head)> &&func);

void block_num_parallel(std::function<void(int n, int n_2x1, int n_1x1)> &&func);

#define HEAD_PARALLEL(impl) \
    ::helper::head_parallel([](uint64_t head) {impl})

#define BLOCK_NUM_PARALLEL(impl) \
    ::helper::block_num_parallel([](int n, int n_2x1, int n_1x1) {impl})

// ----------------------------------------------------------------------------------------- //

/// Spawn all valid Groups in parallel.
void group_parallel(std::function<void(Group group)> &&func);

/// Spawn all valid type_ids in parallel.
void type_id_parallel(std::function<void(uint32_t type_id)> &&func);

/// Spawn all valid GroupUnions in parallel.
void group_union_parallel(std::function<void(GroupUnion group_union)> &&func);

#define GROUP_PARALLEL(impl) \
    ::helper::group_parallel([](Group group) {impl})

#define TYPE_ID_PARALLEL(impl) \
    ::helper::type_id_parallel([](uint32_t type_id) {impl})

#define GROUP_UNION_PARALLEL(impl) \
    ::helper::group_union_parallel([](GroupUnion group_union) {impl})

// ----------------------------------------------------------------------------------------- //

/// Spawn all valid RawCodes in parallel.
void raw_code_parallel(std::function<void(std::span<RawCode>)> &&func);

/// Spawn all valid ShortCodes in parallel.
void short_code_parallel(std::function<void(std::span<ShortCode>)> &&func);

/// Spawn all valid CommonCodes in parallel.
void common_code_parallel(std::function<void(std::span<CommonCode>)> &&func);

#define CODE_PARALLEL(Type, type, impl)                              \
    ::helper::type##_code_parallel([](std::span<Type##Code> codes) { \
        for (auto code : codes) {impl}                               \
    })

#define RAW_CODE_PARALLEL(impl) CODE_PARALLEL(Raw, raw, impl)
#define SHORT_CODE_PARALLEL(impl) CODE_PARALLEL(Short, short, impl)
#define COMMON_CODE_PARALLEL(impl) CODE_PARALLEL(Common, common, impl)

// ----------------------------------------------------------------------------------------- //

/// Calculate multiple scopes separately and combine the results.
template<typename T, typename F>
requires std::is_integral_v<T> && std::is_invocable_v<F, T, T> // func(start, end)
auto scope_parallel(T limit, F &&func) -> std::invoke_result_t<F, T, T>;

#define SCOPE_PARALLEL(limit, impl) \
    ::helper::scope_parallel(limit, impl)

// ----------------------------------------------------------------------------------------- //

} // namespace helper

#include "internal/parallel.inl"
