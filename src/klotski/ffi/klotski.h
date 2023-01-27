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
    extern bool raw_code_check(uint64_t code);
    extern bool short_code_check(uint32_t code);
    extern bool common_code_check(uint64_t code);

    extern void short_code_speed_up();
    extern void short_code_speed_up_fast();
#ifdef __cplusplus
}
#endif
