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
    EXTERN_FUNC void tmain();
#ifdef __cplusplus
}
#endif

/// ---------------------------------- klotski metadata info ----------------------------------

#ifdef __cplusplus
extern "C" {
#endif
    extern uint32_t get_version_major();
    EXTERN_FUNC uint32_t get_version_minor();
    EXTERN_FUNC uint32_t get_version_patch();

    EXTERN_FUNC const char* get_author();
    EXTERN_FUNC const char* get_git_tag();
    EXTERN_FUNC const char* get_version();
    EXTERN_FUNC const char* get_commit_id();
    EXTERN_FUNC const char* get_build_time();
    EXTERN_FUNC const char* get_git_branch();
    EXTERN_FUNC const char* get_project_url();
    EXTERN_FUNC const char* get_system_info();
    EXTERN_FUNC const char* get_compiler_info();
#ifdef __cplusplus
}
#endif

/// -------------------------------- klotski all cases export ---------------------------------

#ifdef __cplusplus
extern "C" {
#endif
    EXTERN_FUNC void all_cases_build();
    EXTERN_FUNC void basic_ranges_build();
    EXTERN_FUNC bool is_all_cases_available();
    EXTERN_FUNC bool is_basic_ranges_available();

    extern const uint32_t ALL_CASES_SIZE;
    EXTERN_FUNC void export_all_cases(uint64_t *buffer);

    extern const uint32_t BASIC_RANGES_SIZE;
    EXTERN_FUNC void export_basic_ranges(uint32_t *buffer);
#ifdef __cplusplus
}
#endif

/// --------------------------------- klotski codec interface ---------------------------------

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
    EXTERN_FUNC void short_code_to_string_unsafe(uint32_t short_code, char short_code_str[]);
    EXTERN_FUNC bool short_code_from_string(const char short_code_str[], uint32_t *short_code);

    extern const uint32_t COMMON_CODE_STR_SIZE;
    EXTERN_FUNC bool common_code_to_string(uint64_t common_code, char common_code_str[]);
    EXTERN_FUNC void common_code_to_string_unsafe(uint64_t common_code, char common_code_str[]);
    EXTERN_FUNC bool common_code_to_string_shorten(uint64_t common_code, char common_code_str[]);
    EXTERN_FUNC void common_code_to_string_shorten_unsafe(uint64_t common_code, char common_code_str[]);
    EXTERN_FUNC bool common_code_from_string(const char common_code_str[], uint64_t *common_code);
#ifdef __cplusplus
}
#endif

/// ------------------------------------ klotski benchmark ------------------------------------

#ifdef __cplusplus
extern "C" {
#endif
    EXTERN_FUNC void benchmark_preparation();
    EXTERN_FUNC double benchmark_warm_up(uint64_t count);

    EXTERN_FUNC double benchmark_range_flip_ns();

    EXTERN_FUNC double benchmark_all_cases_ms();
    EXTERN_FUNC double benchmark_basic_ranges_ms();

    EXTERN_FUNC double benchmark_raw_code_check_ns();
    EXTERN_FUNC double benchmark_short_code_check_ns();
    EXTERN_FUNC double benchmark_common_code_check_ns();
    EXTERN_FUNC double benchmark_raw_code_check_random_ns();
    EXTERN_FUNC double benchmark_short_code_check_random_ns();
    EXTERN_FUNC double benchmark_common_code_check_random_ns();

    EXTERN_FUNC double benchmark_short_code_to_string_ns();
    EXTERN_FUNC double benchmark_short_code_from_string_ns();
    EXTERN_FUNC double benchmark_common_code_to_string_ns();
    EXTERN_FUNC double benchmark_common_code_from_string_ns();

    EXTERN_FUNC double benchmark_common_code_to_raw_code_ns();
    EXTERN_FUNC double benchmark_raw_code_to_common_code_ns();
    EXTERN_FUNC double benchmark_common_code_to_short_code_us();
    EXTERN_FUNC double benchmark_short_code_to_common_code_us();
    EXTERN_FUNC double benchmark_common_code_to_short_code_fast_ns();
    EXTERN_FUNC double benchmark_short_code_to_common_code_fast_ns();

    EXTERN_FUNC double benchmark_vertical_mirror_check_ns();
    EXTERN_FUNC double benchmark_horizontal_mirror_check_ns();
    EXTERN_FUNC double benchmark_vertical_mirror_convert_ns();
    EXTERN_FUNC double benchmark_horizontal_mirror_convert_ns();
#ifdef __cplusplus
}
#endif
