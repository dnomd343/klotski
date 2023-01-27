#pragma once

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#include <stdbool.h>
#endif

/// klotski test interface
#ifdef __cplusplus
extern "C" {
#endif
    // TODO: remove after test
    extern void tmain();
#ifdef __cplusplus
}
#endif

/// klotski codec interface
#ifdef __cplusplus
extern "C" {
#endif
    extern void short_code_enable();
    extern void short_code_enable_fast();
    extern bool is_short_code_available();
    extern bool is_short_code_available_fast();

    extern bool raw_code_check(uint64_t raw_code);
    extern bool short_code_check(uint32_t short_code);
    extern bool common_code_check(uint64_t common_code);

    // TODO: string code convert

    extern bool raw_code_to_short_code(uint64_t raw_code, uint32_t *short_code);
    extern bool short_code_to_raw_code(uint32_t short_code, uint64_t *raw_code);
    extern bool raw_code_to_common_code(uint64_t raw_code, uint64_t *common_code);
    extern bool common_code_to_raw_code(uint64_t common_code, uint64_t *raw_code);
    extern bool short_code_to_common_code(uint32_t short_code, uint64_t *common_code);
    extern bool common_code_to_short_code(uint64_t common_code, uint32_t *short_code);

#ifdef __cplusplus
}
#endif
