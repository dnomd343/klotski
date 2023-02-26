#pragma once

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#include <stdbool.h>
#endif

#define EXTERN_FUNC extern

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
    EXTERN_FUNC void short_code_enable();
    EXTERN_FUNC void short_code_enable_fast();
    EXTERN_FUNC bool is_short_code_available();
    EXTERN_FUNC bool is_short_code_available_fast();

    EXTERN_FUNC bool raw_code_check(uint64_t raw_code);
    EXTERN_FUNC bool short_code_check(uint32_t short_code);
    EXTERN_FUNC bool common_code_check(uint64_t common_code);

    EXTERN_FUNC bool raw_code_to_short_code(uint64_t raw_code, uint32_t *short_code);
    EXTERN_FUNC bool short_code_to_raw_code(uint32_t short_code, uint64_t *raw_code);
    EXTERN_FUNC bool raw_code_to_common_code(uint64_t raw_code, uint64_t *common_code);
    EXTERN_FUNC bool common_code_to_raw_code(uint64_t common_code, uint64_t *raw_code);
    EXTERN_FUNC bool short_code_to_common_code(uint32_t short_code, uint64_t *common_code);
    EXTERN_FUNC bool common_code_to_short_code(uint64_t common_code, uint32_t *short_code);

    EXTERN_FUNC uint32_t raw_code_to_short_code_unsafe(uint64_t raw_code);
    EXTERN_FUNC uint64_t short_code_to_raw_code_unsafe(uint32_t short_code);
    EXTERN_FUNC uint64_t raw_code_to_common_code_unsafe(uint64_t raw_code);
    EXTERN_FUNC uint64_t common_code_to_raw_code_unsafe(uint64_t common_code);
    EXTERN_FUNC uint64_t short_code_to_common_code_unsafe(uint32_t short_code);
    EXTERN_FUNC uint32_t common_code_to_short_code_unsafe(uint64_t common_code);

    EXTERN_FUNC bool is_vertical_mirror(uint64_t raw_code, bool *result);
    EXTERN_FUNC bool is_horizontal_mirror(uint64_t raw_code, bool *result);
    EXTERN_FUNC bool to_vertical_mirror(uint64_t raw_code, uint64_t *result);
    EXTERN_FUNC bool to_horizontal_mirror(uint64_t raw_code, uint64_t *result);

    EXTERN_FUNC bool is_vertical_mirror_unsafe(uint64_t raw_code);
    EXTERN_FUNC bool is_horizontal_mirror_unsafe(uint64_t raw_code);
    EXTERN_FUNC uint64_t to_vertical_mirror_unsafe(uint64_t raw_code);
    EXTERN_FUNC uint64_t to_horizontal_mirror_unsafe(uint64_t raw_code);

    extern const uint32_t SHORT_CODE_STR_SIZE;
    EXTERN_FUNC bool short_code_to_string(uint32_t short_code, char short_code_str[]);
    EXTERN_FUNC bool short_code_from_string(const char short_code_str[], uint32_t *short_code);

    extern const uint32_t COMMON_CODE_STR_SIZE;
    EXTERN_FUNC bool common_code_to_string(uint64_t common_code, char common_code_str[]);
    EXTERN_FUNC bool common_code_to_string_shorten(uint64_t common_code, char common_code_str[]);
    EXTERN_FUNC bool common_code_from_string(const char common_code_str[], uint64_t *common_code);
#ifdef __cplusplus
}
#endif
