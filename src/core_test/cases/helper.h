#pragma once

#include "exposer.h"
#include "all_cases.h"
#include "concurrent.h"
#include "gtest/gtest.h"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::cases::ALL_CASES_NUM;
using klotski::cases::ALL_CASES_NUM_;
using klotski::cases::BASIC_RANGES_NUM;

/// Test fixture wrapper with Racer and Executor.
class Concurrent {
protected:
    co::Racer racer_;
    co::Executor executor_;
};

/// Forcibly modify private variables to reset state.
FORCIBLY_ACCESS(AllCases, available_, bool)
FORCIBLY_ACCESS(BasicRanges, available_, bool)

/// Test fixture macro with custom test suite name.
#define TEST_FF(test_suite_name, test_name)                        \
    GTEST_TEST_(test_suite_name, test_name, test_suite_name##Test, \
                ::testing::internal::GetTypeId<test_suite_name##Test>())
