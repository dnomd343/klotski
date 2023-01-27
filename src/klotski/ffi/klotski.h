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

/// -------------------------------- klotski codec interface --------------------------------

#ifdef __cplusplus
extern "C" {
#endif
    /// short code warm up api
    extern void short_code_enable();
    extern void short_code_enable_fast();
    extern bool is_short_code_available();
    extern bool is_short_code_available_fast();

    extern bool raw_code_check(uint64_t raw_code);
    extern bool short_code_check(uint32_t short_code);
    extern bool common_code_check(uint64_t common_code);

    extern bool raw_code_to_short_code(uint64_t raw_code, uint32_t *short_code);
    extern bool short_code_to_raw_code(uint32_t short_code, uint64_t *raw_code);
    extern bool raw_code_to_common_code(uint64_t raw_code, uint64_t *common_code);
    extern bool common_code_to_raw_code(uint64_t common_code, uint64_t *raw_code);
    extern bool short_code_to_common_code(uint32_t short_code, uint64_t *common_code);
    extern bool common_code_to_short_code(uint64_t common_code, uint32_t *short_code);

    /// output char[] buffer at least 6-bytes
    extern const uint32_t short_code_string_size;
    extern bool short_code_to_string(uint32_t short_code, char short_code_str[]);
    extern bool short_code_from_string(const char short_code_str[], uint32_t *short_code);

    /// output char[] buffer at least 10-bytes
    extern const uint32_t common_code_string_size;
    extern bool common_code_to_string(uint64_t common_code, char common_code_str[]);
    extern bool common_code_to_string_shorten(uint64_t common_code, char common_code_str[]);
    extern bool common_code_from_string(const char common_code_str[], uint64_t *common_code);

#ifdef __cplusplus
}
#endif
