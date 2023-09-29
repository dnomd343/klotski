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

/// Perform the build of all_cases, it is blocking, and will return directly
/// if completed.
EXTERN void all_cases_build();

/// Execute the asynchronous build of all_cases, the task will be sent to the
/// executor, and the callback will be called after completion. Even if the
/// data is ready, the callback will still be triggered.
EXTERN void all_cases_build_async(executor_t executor, notifier_t callback);

/// Build all_cases in parallel, the tasks will be split and sent to the
/// executor, you can put them on different threads to work, but note that the
/// task can only be executed once, otherwise it will lead to unknown
/// consequences, the function will be blocked until all mission completed.
EXTERN void all_cases_build_parallel(executor_t executor);

/// Similar to `all_cases_build_parallel`, but it is non-blocking. The callback
/// will be triggered after the build is completed. Note that the callback will
/// still be triggered even if the data is ready.
EXTERN void all_cases_build_parallel_async(executor_t executor, notifier_t callback);

/// Returns whether the all_cases is ready, 0 means not completed, non-0 means
/// the data is ready.
EXTERN int is_all_cases_available();

//extern const uint32_t ALL_CASES_SIZE;
//EXTERN_FUNC void export_all_cases(uint64_t *buffer);

//extern const uint32_t BASIC_RANGES_SIZE;
//EXTERN_FUNC void export_basic_ranges(uint32_t *buffer);
#ifdef __cplusplus
}
#endif

#endif /* KLOTSKI_H_ */
