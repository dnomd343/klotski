#include <printf.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "core/ffi/klotski.h"

struct pthread_wrapper_t {
    void (*fn)(void*);
    void *arg;
};

void* pthread_wrapper(void *arg) {

    printf("enter wrapper\n");

    struct pthread_wrapper_t *kk = (struct pthread_wrapper_t*)arg;

    kk->fn(kk->arg);

    return NULL;
}

void callback() {
    printf("enter callback\n");
}

void executor(void(*fn)(void*), void *arg) {
    printf("executor receive task\n");
//    fn(arg);

    struct pthread_wrapper_t *kk = (struct pthread_wrapper_t*)malloc(sizeof(struct pthread_wrapper_t));

    kk->fn = fn;
    kk->arg = arg;

    pthread_t pp;
    pthread_create(&pp, NULL, pthread_wrapper, (void*)kk);

}

int main() {
    printf("prebuild available -> %d\n", is_all_cases_prebuild_available());

//    printf("prebuild begin\n");
//    all_cases_prebuild();
//    printf("prebuild complete\n");

    printf("prebuild begin\n");
    all_cases_prebuild_async(executor, callback);
    printf("prebuild func exited\n");
    printf("prebuild available -> %d\n", is_all_cases_prebuild_available());

    printf("start sleep 3s\n");
    sleep(3);

    printf("prebuild available -> %d\n", is_all_cases_prebuild_available());

    return 0;
}
