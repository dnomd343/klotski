#pragma once

#ifdef __cplusplus
#include <cstdint>
#define NOEXCEPT noexcept
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
    EXTERN_FUNC void tmain();
#ifdef __cplusplus
}
#endif

/// -------------------------------- klotski metadata info ----------------------------------

#ifdef __cplusplus
extern "C" {
#endif
    EXTERN_FUNC uint32_t get_version_major() NOEXCEPT;
    EXTERN_FUNC uint32_t get_version_minor() NOEXCEPT;
    EXTERN_FUNC uint32_t get_version_patch() NOEXCEPT;

    EXTERN_FUNC const char* get_author() NOEXCEPT;
    EXTERN_FUNC const char* get_git_tag() NOEXCEPT;
    EXTERN_FUNC const char* get_version() NOEXCEPT;
    EXTERN_FUNC const char* get_commit_id() NOEXCEPT;
    EXTERN_FUNC const char* get_build_time() NOEXCEPT;
    EXTERN_FUNC const char* get_git_branch() NOEXCEPT;
    EXTERN_FUNC const char* get_project_url() NOEXCEPT;
    EXTERN_FUNC const char* get_system_info() NOEXCEPT;
    EXTERN_FUNC const char* get_compiler_info() NOEXCEPT;
#ifdef __cplusplus
}
#endif

/// ------------------------------- klotski all cases export --------------------------------

#ifdef __cplusplus
extern "C" {
#endif
    EXTERN_FUNC void all_cases_build() NOEXCEPT;
    EXTERN_FUNC void basic_ranges_build() NOEXCEPT;
    EXTERN_FUNC bool is_all_cases_available() NOEXCEPT;
    EXTERN_FUNC bool is_basic_ranges_available() NOEXCEPT;

    extern const uint32_t ALL_CASES_SIZE;
    EXTERN_FUNC void export_all_cases(uint64_t *buffer) NOEXCEPT;

    extern const uint32_t BASIC_RANGES_SIZE;
    EXTERN_FUNC void export_basic_ranges(uint32_t *buffer) NOEXCEPT;
#ifdef __cplusplus
}
#endif

/// -------------------------------- klotski codec interface --------------------------------

#ifdef __cplusplus
extern "C" {
#endif
    EXTERN_FUNC void short_code_enable() NOEXCEPT;
    EXTERN_FUNC void short_code_enable_fast() NOEXCEPT;
    EXTERN_FUNC bool is_short_code_available() NOEXCEPT;
    EXTERN_FUNC bool is_short_code_available_fast() NOEXCEPT;

    EXTERN_FUNC bool raw_code_check(uint64_t raw_code) NOEXCEPT;
    EXTERN_FUNC bool short_code_check(uint32_t short_code) NOEXCEPT;
    EXTERN_FUNC bool common_code_check(uint64_t common_code) NOEXCEPT;

    EXTERN_FUNC bool raw_code_to_short_code(uint64_t raw_code, uint32_t *short_code) NOEXCEPT;
    EXTERN_FUNC bool short_code_to_raw_code(uint32_t short_code, uint64_t *raw_code) NOEXCEPT;
    EXTERN_FUNC bool raw_code_to_common_code(uint64_t raw_code, uint64_t *common_code) NOEXCEPT;
    EXTERN_FUNC bool common_code_to_raw_code(uint64_t common_code, uint64_t *raw_code) NOEXCEPT;
    EXTERN_FUNC bool short_code_to_common_code(uint32_t short_code, uint64_t *common_code) NOEXCEPT;
    EXTERN_FUNC bool common_code_to_short_code(uint64_t common_code, uint32_t *short_code) NOEXCEPT;

    EXTERN_FUNC uint32_t raw_code_to_short_code_unsafe(uint64_t raw_code) NOEXCEPT;
    EXTERN_FUNC uint64_t short_code_to_raw_code_unsafe(uint32_t short_code) NOEXCEPT;
    EXTERN_FUNC uint64_t raw_code_to_common_code_unsafe(uint64_t raw_code) NOEXCEPT;
    EXTERN_FUNC uint64_t common_code_to_raw_code_unsafe(uint64_t common_code) NOEXCEPT;
    EXTERN_FUNC uint64_t short_code_to_common_code_unsafe(uint32_t short_code) NOEXCEPT;
    EXTERN_FUNC uint32_t common_code_to_short_code_unsafe(uint64_t common_code) NOEXCEPT;

    EXTERN_FUNC bool is_vertical_mirror(uint64_t raw_code, bool *result) NOEXCEPT;
    EXTERN_FUNC bool is_horizontal_mirror(uint64_t raw_code, bool *result) NOEXCEPT;
    EXTERN_FUNC bool to_vertical_mirror(uint64_t raw_code, uint64_t *result) NOEXCEPT;
    EXTERN_FUNC bool to_horizontal_mirror(uint64_t raw_code, uint64_t *result) NOEXCEPT;

    EXTERN_FUNC bool is_vertical_mirror_unsafe(uint64_t raw_code) NOEXCEPT;
    EXTERN_FUNC bool is_horizontal_mirror_unsafe(uint64_t raw_code) NOEXCEPT;
    EXTERN_FUNC uint64_t to_vertical_mirror_unsafe(uint64_t raw_code) NOEXCEPT;
    EXTERN_FUNC uint64_t to_horizontal_mirror_unsafe(uint64_t raw_code) NOEXCEPT;

    extern const uint32_t SHORT_CODE_STR_SIZE;
    EXTERN_FUNC bool short_code_to_string(uint32_t short_code, char short_code_str[]) NOEXCEPT;
    EXTERN_FUNC bool short_code_from_string(const char short_code_str[], uint32_t *short_code) NOEXCEPT;

    extern const uint32_t COMMON_CODE_STR_SIZE;
    EXTERN_FUNC bool common_code_to_string(uint64_t common_code, char common_code_str[]) NOEXCEPT;
    EXTERN_FUNC bool common_code_to_string_shorten(uint64_t common_code, char common_code_str[]) NOEXCEPT;
    EXTERN_FUNC bool common_code_from_string(const char common_code_str[], uint64_t *common_code) NOEXCEPT;
#ifdef __cplusplus
}
#endif
