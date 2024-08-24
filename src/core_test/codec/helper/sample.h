#pragma once

#include <string>
#include <cstdint>

// ----------------------------------------------------------------------------------------- //

/// Valid klotski RawCode
constexpr uint64_t TEST_R_CODE = 0x0603'EDF5'CAFF'F5E2;

/// Valid klotski ShortCode
constexpr uint32_t TEST_S_CODE = 4091296;
const std::string TEST_S_CODE_STR = "4WVE1"; // TODO: using `std::string_view`
#define TEST_S_CODE_STR_RV std::string(TEST_S_CODE_STR) // TODO: remove r-value

/// Valid klotski CommonCode
constexpr uint64_t TEST_C_CODE = 0x1'A9BF'0C00;
const std::string TEST_C_CODE_STR = "1A9BF0C00";
#define TEST_C_CODE_STR_RV std::string(TEST_C_CODE_STR) // TODO: remove r-value

// ----------------------------------------------------------------------------------------- //

/// Invalid klotski RawCode
constexpr uint64_t TEST_R_CODE_ERR = 0x1603'ED00'CAFF'F5E2;

/// Invalid klotski ShortCode
constexpr uint32_t TEST_S_CODE_ERR = 1234564323;
const std::string TEST_S_CODE_STR_ERR = "ZZZZZZ";
#define TEST_S_CODE_STR_ERR_RV std::string(TEST_S_CODE_STR_ERR) // TODO: remove r-value

/// Invalid klotski CommonCode
constexpr uint64_t TEST_C_CODE_ERR = 0x3'A9BF'0C00;
const static std::string TEST_C_CODE_STR_ERR = "0123456789";
#define TEST_C_CODE_STR_ERR_RV std::string(TEST_C_CODE_STR_ERR) // TODO: remove r-value

// ----------------------------------------------------------------------------------------- //
