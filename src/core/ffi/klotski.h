#ifndef KLOTSKI_H_
#define KLOTSKI_H_

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

#define KLOTSKI_TRUE  1
#define KLOTSKI_FALSE 0

#define EXTERN extern

typedef void (*notifier_t)();
typedef void (*executor_t)(void (*fn)(void*), void *arg);

#ifdef __cplusplus
extern "C" {
#endif

/// Execute all_cases pre-build work, blocking until the end of the build. If
/// the build has been completed, the function will return directly.
EXTERN void all_cases_prebuild();

/// Execute all_cases pre-build work, the function will return directly without
/// blocking, and will be notified with callback when completed. If the build
/// has been completed, the parameter function will still be called.
EXTERN void all_cases_prebuild_async(executor_t executor, notifier_t callback);

/// Returns the pre-build status of all_cases without any block, value is 0 if
/// not completed, non-0 otherwise.
EXTERN int is_all_cases_prebuild_available();

EXTERN void all_cases_build();

EXTERN void all_cases_build_async(executor_t executor, notifier_t callback);

EXTERN void all_cases_parallel_build(executor_t executor);

EXTERN void all_cases_parallel_build_async(executor_t executor, notifier_t callback);

EXTERN int is_all_cases_available();

//extern const uint32_t ALL_CASES_SIZE;
//EXTERN_FUNC void export_all_cases(uint64_t *buffer);

//extern const uint32_t BASIC_RANGES_SIZE;
//EXTERN_FUNC void export_basic_ranges(uint32_t *buffer);
#ifdef __cplusplus
}
#endif

#endif /* KLOTSKI_H_ */
