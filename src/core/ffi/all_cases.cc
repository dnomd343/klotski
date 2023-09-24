#include "klotski.h"
#include "all_cases.h"

#include <iostream>
#include <future>

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

void all_cases_prebuild() {
    BasicRanges::Instance().Build();
}

void all_cases_prebuild_async(executor_t executor, notifier_t callback) {
    executor([](void *cb) {
        all_cases_prebuild();
        ((notifier_t)cb)();
    }, (void*)callback);
}

int is_all_cases_prebuild_available() {
    if (BasicRanges::Instance().IsAvailable()) {
        return KLOTSKI_TRUE;
    } else {
        return KLOTSKI_FALSE;
    }
}
