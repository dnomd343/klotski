#include "klotski.h"
#include "all_cases/all_cases.h"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;
using klotski::cases::ALL_CASES_NUM;

typedef std::function<void()> Task;

// ------------------------------------------------------------------------------------- //

void all_cases_prebuild() {
    BasicRanges::instance().build();
}

void all_cases_prebuild_async(const executor_t executor, const notifier_t callback) {
    auto worker = [executor](Task &&task) {
        const auto func = [](void *arg) {
            const auto *ptr = static_cast<Task*>(arg);
            (*ptr)();
            delete ptr;
        };
        executor(func, new Task {std::move(task)});
    };
    BasicRanges::instance().build_async(worker, callback);
}

int all_cases_prebuild_available() {
    return BasicRanges::instance().is_available() ? KLSK_TRUE : KLSK_FALSE;
}

// ------------------------------------------------------------------------------------- //

void all_cases_build() {
    AllCases::instance().build();
}

void all_cases_build_async(const executor_t executor, const notifier_t callback) {
    auto worker = [executor](Task &&task) {
        const auto func = [](void *arg) {
            const auto *ptr = static_cast<Task*>(arg);
            (*ptr)();
            delete ptr;
        };
        executor(func, new Task {std::move(task)});
    };
    AllCases::instance().build_async(worker, callback);
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
    return AllCases::instance().fetch().ranges(head).data();
}

// ------------------------------------------------------------------------------------- //
