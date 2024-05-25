#include "klotski.h"
#include "all_cases/all_cases.h"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;
using klotski::cases::ALL_CASES_NUM;

typedef std::function<void()> Runner;

// ------------------------------------------------------------------------------------- //

void all_cases_prebuild() {
    BasicRanges::instance().build();
}

void all_cases_prebuild_async(const executor_t executor, const notifier_t callback) {
    const auto func = [](void *arg) {
        all_cases_prebuild();
        reinterpret_cast<notifier_t>(arg)();
    };
    executor(func, reinterpret_cast<void*>(callback));
}

int all_cases_prebuild_available() {
    return BasicRanges::instance().is_available() ? KLSK_TRUE : KLSK_FALSE;
}

// ------------------------------------------------------------------------------------- //

void all_cases_build() {
    AllCases::instance().build();
}

void all_cases_build_async(const executor_t executor, const notifier_t callback) {
    const auto func = [](void *arg) {
        all_cases_build();
        reinterpret_cast<notifier_t>(arg)();
    };
    executor(func, reinterpret_cast<void*>(callback));
}

void all_cases_build_parallel(executor_t executor) {
    // AllCases::instance().build_parallel([executor](Runner &&runner) {
    //     const auto func = [](void *arg) {
    //         (*static_cast<Runner*>(arg))();
    //         delete static_cast<Runner*>(arg);
    //     };
    //     executor(func, new Runner {std::move(runner)});
    // });
}

void all_cases_build_parallel_async(executor_t executor, notifier_t callback) {
    auto all_done = [callback] {
        callback();
    };
    AllCases::instance().build_parallel_async([executor](Runner &&runner) {
        const auto func = [](void *arg) {
            (*static_cast<Runner*>(arg))();
            delete static_cast<Runner*>(arg);
        };
        executor(func, new Runner {std::move(runner)});
    }, std::move(all_done));
}

int all_cases_available() {
    return AllCases::instance().is_available() ? KLSK_TRUE : KLSK_FALSE;
}

// ------------------------------------------------------------------------------------- //

int all_cases_num(const int head) {
    if (head < 0 || head >= static_cast<int>(ALL_CASES_NUM.size())) {
        return -1;
    }
    return ALL_CASES_NUM[head];
}

const uint32_t* all_cases_export(const int head) {
    if (all_cases_num(head) < 0) {
        return nullptr;
    }
    return AllCases::instance().fetch()[head].data();
}

// ------------------------------------------------------------------------------------- //
