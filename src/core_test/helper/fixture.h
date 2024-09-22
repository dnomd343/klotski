#pragma once

#include <atomic>
#include <gtest/gtest.h>

#include "helper/concurrent.h"

namespace helper {

/// Test fixture wrapper with concurrency tools.
class Concurrent {
protected:
    // Execute same task concurrently.
    Racer racer_ {};

    // Execute assigned tasks one by one.
    Executor serial_ {1};

    // Execute assigned tasks on all cores.
    Executor executor_ {0};

    // Atomic helpers for multi-thread testing.
    std::atomic<int> counter_ {0};
    std::atomic_flag condition_ {false};
};

/// Test fixture macro with custom test suite name.
#define TEST_FF(test_suite_name, test_name)                        \
    GTEST_TEST_(test_suite_name, test_name, test_suite_name##Test, \
                ::testing::internal::GetTypeId<test_suite_name##Test>())

} // namespace helper
