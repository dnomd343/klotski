#include <printf.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "core/ffi/klotski.h"

typedef struct {
    void (*fn)(void*);
    void *arg;
} pthread_wrapper_t;

void* pthread_wrapper(void *arg) {
    printf("wrapper at %p\n", pthread_self());
    pthread_wrapper_t *kk = (pthread_wrapper_t*)arg;
    kk->fn(kk->arg);
    return NULL;
}

void callback() {
    printf("callback is triggered\n");
}

void executor(void(*fn)(void*), void *arg) {
    printf("executor receive task\n");
//    fn(arg);

    pthread_wrapper_t *kk = (pthread_wrapper_t*)malloc(sizeof(pthread_wrapper_t));
    kk->fn = fn;
    kk->arg = arg;

    pthread_t pp;
    pthread_create(&pp, NULL, pthread_wrapper, (void*)kk);
}

int main() {
//    printf("prebuild available -> %d\n", all_cases_prebuild_available());
//
//    printf("prebuild begin\n");
//    all_cases_prebuild();
//    printf("prebuild complete\n");
//
//    printf("prebuild begin\n");
//    all_cases_prebuild_async(executor, callback);
//    printf("prebuild func exited\n");
//    printf("prebuild available -> %d\n", all_cases_prebuild_available());
//    printf("start sleep 3s\n");
//    sleep(3);
//
//    printf("prebuild available -> %d\n", all_cases_prebuild_available());


//    printf("build available -> %d\n", all_cases_available());
//
//    printf("build begin\n");
//    all_cases_build();
//    printf("build complete\n");
//
//    printf("build begin\n");
//    all_cases_build_async(executor, callback);
//    printf("build func exited\n");
//    printf("build available -> %d\n", all_cases_available());
//    printf("start sleep 3s\n");
//    sleep(3);
//
//    printf("build begin\n");
//    all_cases_build_parallel(executor);
//    printf("build complete\n");
//
//    printf("build begin\n");
//    all_cases_build_parallel_async(executor, callback);
//    printf("build func exited\n");
//    printf("build available -> %d\n", all_cases_available());
//    printf("start sleep 3s\n");
//    sleep(3);
//
//    printf("build available -> %d\n", all_cases_available());


//    int i = -2;
//    for (; i < 18; ++i) {
//        printf("all cases %d -> %d (%p)\n", i, all_cases_num(i), all_cases_export(i));
//    }

    int i = 0;
    for (; i < 16; ++i) {
        int j = 0;
        const klotski_u32 *ranges = all_cases_export(i);
        for (; j < all_cases_num(i); ++j) {
            printf("%X%08X\n", i, ranges[j]);
        }
    }

    return 0;
}
