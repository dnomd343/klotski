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

void all_cases_build() {
    AllCases::Instance().Build();
}

void all_cases_build_async(executor_t executor, notifier_t callback) {
    executor([](void *cb) {
        all_cases_build();
        ((notifier_t)cb)();
    }, (void*)callback);
}

void all_cases_parallel_build(executor_t executor) {
    typedef std::function<void()> Runner;
    AllCases::Instance().BuildParallel([executor](Runner &&runner) {
        executor([](void *fn) {
            (*(Runner*)fn)();
            delete (Runner*)fn;
        }, (void*)new Runner{std::move(runner)});
    });
}

void all_cases_parallel_build_async(executor_t executor, notifier_t callback) {

    typedef std::pair<executor_t, notifier_t> pp_t;

    auto pp = new pp_t;
    pp->first = executor;
    pp->second = callback;


    auto lambda = [](void *arg) {

        std::cout << "enter lambda" << std::endl;

        auto *pp = (pp_t*)arg;

        all_cases_parallel_build(pp->first);

        ((notifier_t)pp->second)();

    };

    executor(lambda, (void*)pp);

}

int is_all_cases_available() {
    if (AllCases::Instance().IsAvailable()) {
        return KLOTSKI_TRUE;
    } else {
        return KLOTSKI_FALSE;
    }
}
