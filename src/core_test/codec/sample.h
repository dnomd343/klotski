#pragma once

#include <string>
#include <cstdint>

#include "all_cases/all_cases.h"

// ----------------------------------------------------------------------------------------- //

// Valid RawCode
const uint64_t TEST_R_CODE = 0x0603'EDF5'CAFF'F5E2;

// Valid ShortCode
const uint32_t TEST_S_CODE = 4091296;
const std::string TEST_S_CODE_STR = "4WVE1";
#define TEST_S_CODE_STR_RV std::string(TEST_S_CODE_STR) // r-value

// Valid CommonCode
const uint64_t TEST_C_CODE = 0x1'A9BF'0C00;
const std::string TEST_C_CODE_STR = "1A9BF0C00";
#define TEST_C_CODE_STR_RV std::string(TEST_C_CODE_STR) // r-value

// ----------------------------------------------------------------------------------------- //

// Invalid RawCode
const static uint64_t TEST_R_CODE_ERR = 0x1603'ED00'CAFF'F5E2;

// Invalid ShortCode
const static uint32_t TEST_S_CODE_ERR = 1234564323;
const static std::string TEST_S_CODE_STR_ERR = "ZZZZZZ";
#define TEST_S_CODE_STR_ERR_RV std::string(TEST_S_CODE_STR_ERR) // r-value

// Invalid CommonCode
const static uint64_t TEST_C_CODE_ERR = 0x3'A9BF'0C00;
const static std::string TEST_C_CODE_STR_ERR = "0123456789";
#define TEST_C_CODE_STR_ERR_RV std::string(TEST_C_CODE_STR_ERR) // r-value

// ----------------------------------------------------------------------------------------- //

using klotski::cases::AllCases;
using klotski::cases::ALL_CASES_NUM_;

inline std::vector<uint64_t> all_common_codes() {
    std::vector<uint64_t> common_codes;
    common_codes.reserve(ALL_CASES_NUM_);
    for (uint64_t head = 0; head < 16; ++head) {
        for (auto range : AllCases::instance().fetch()[head]) {
            common_codes.emplace_back(head << 32 | range);
        }
    }
    return common_codes;
}

// ----------------------------------------------------------------------------------------- //
