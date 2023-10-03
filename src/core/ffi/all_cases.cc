#include "klotski.h"
#include "all_cases.h"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;
using klotski::cases::ALL_CASES_NUM;

void all_cases_prebuild() {
    BasicRanges::instance().build();
}

void all_cases_prebuild_async(executor_t executor, notifier_t callback) {
    executor([](void *fn) {
        all_cases_prebuild();
        ((notifier_t)fn)();
    }, (void*)callback);
}

int all_cases_prebuild_available() {
    return BasicRanges::instance().is_available() ? KLOTSKI_TRUE : KLOTSKI_FALSE;
}

void all_cases_build() {
    AllCases::instance().build();
}

void all_cases_build_async(executor_t executor, notifier_t callback) {
    executor([](void *fn) {
        all_cases_build();
        ((notifier_t)fn)();
    }, (void*)callback);
}

void all_cases_build_parallel(executor_t executor) {
    typedef std::function<void()> Runner;
    AllCases::instance().build_parallel([executor](Runner &&runner) {
        executor([](void *fn) {
            (*(Runner*)fn)();
            delete (Runner*)fn;
        }, (void*)new Runner{std::move(runner)});
    });
}

void all_cases_build_parallel_async(executor_t executor, notifier_t callback) {
    typedef std::function<void()> Runner;
    auto all_done = [callback]() {
        callback();
    };
    AllCases::instance().build_parallel_async([executor](Runner &&runner) {
        executor([](void *fn) {
            (*(Runner*)fn)();
            delete (Runner*)fn;
        }, (void*)new Runner{std::move(runner)});
    }, std::move(all_done));
}

int all_cases_available() {
    return AllCases::instance().is_available() ? KLOTSKI_TRUE : KLOTSKI_FALSE;
}

int all_cases_num(int head) {
    if (head < 0 || head >= (int)ALL_CASES_NUM.size()) {
        return -1;
    }
    return ALL_CASES_NUM[head];
}

const klotski_u32* all_cases_export(int head) {
    if (all_cases_num(head) < 0) {
        return nullptr;
    }
    return AllCases::instance().fetch()[head].data();
}
