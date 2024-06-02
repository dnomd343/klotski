#pragma once

#include <algorithm>

#include "exposer.h"
#include "all_cases.h"
#include "concurrent.h"
#include "gtest/gtest.h"

using klotski::array_sum;

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::cases::ALL_CASES_NUM;
using klotski::cases::ALL_CASES_NUM_;

using klotski::cases::BASIC_RANGES_NUM;
using klotski::cases::BASIC_RANGES_NUM_;

/// Test fixture wrapper with concurrency tools.
class Concurrent {
protected:
    co::Racer racer_ {256};

    co::Executor serial_ {1};
    co::Executor executor_ {0};

    std::atomic<int> counter_ {0};
    std::atomic_flag condition_ {false};
};

/// Forcibly modify private variables to reset state.
FORCIBLY_ACCESS(AllCases, available_, bool)
FORCIBLY_ACCESS(BasicRanges, available_, bool)

/// Test fixture macro with custom test suite name.
#define TEST_FF(test_suite_name, test_name)                        \
    GTEST_TEST_(test_suite_name, test_name, test_suite_name##Test, \
                ::testing::internal::GetTypeId<test_suite_name##Test>())
