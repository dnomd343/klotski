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

/// -------------------------------- klotski metadata info ----------------------------------

#ifdef __cplusplus
extern "C" {
#endif
    // version -> major.minor.patch
    extern uint32_t get_version_major();
    extern uint32_t get_version_minor();
    extern uint32_t get_version_patch();

    extern const char* get_author();
    extern const char* get_git_tag();
    extern const char* get_version();
    extern const char* get_commit_id();
    extern const char* get_build_time();
    extern const char* get_git_branch();
    extern const char* get_project_url();
    extern const char* get_system_info();
    extern const char* get_compiler_info();
#ifdef __cplusplus
}
#endif

/// ------------------------------- klotski all cases export --------------------------------

#ifdef __cplusplus
extern "C" {
#endif
    extern void all_cases_build();
    extern void basic_ranges_build();
    extern bool is_all_cases_available();
    extern bool is_basic_ranges_available();

    extern const uint32_t ALL_CASES_SIZE;
    extern void export_all_cases(uint64_t *buffer);

    extern const uint32_t BASIC_RANGES_SIZE;
    extern void export_basic_ranges(uint32_t *buffer);
#ifdef __cplusplus
}
#endif

/// -------------------------------- klotski codec interface --------------------------------

#ifdef __cplusplus
extern "C" {
#endif
    // short code index warm up
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

    // TODO: allow unsafe convert
    extern uint32_t raw_code_to_short_code_unsafe(uint64_t raw_code);
    extern uint64_t short_code_to_raw_code_unsafe(uint32_t short_code);
    extern uint64_t raw_code_to_common_code_unsafe(uint64_t raw_code);
    extern uint64_t common_code_to_raw_code_unsafe(uint64_t common_code);
    extern uint64_t short_code_to_common_code_unsafe(uint32_t short_code);
    extern uint32_t common_code_to_short_code_unsafe(uint64_t common_code);

    extern const uint32_t SHORT_CODE_STR_SIZE;
    extern bool short_code_to_string(uint32_t short_code, char short_code_str[]);
    extern bool short_code_from_string(const char short_code_str[], uint32_t *short_code);

    extern const uint32_t COMMON_CODE_STR_SIZE;
    extern bool common_code_to_string(uint64_t common_code, char common_code_str[]);
    extern bool common_code_to_string_shorten(uint64_t common_code, char common_code_str[]);
    extern bool common_code_from_string(const char common_code_str[], uint64_t *common_code);
#ifdef __cplusplus
}
#endif
