#pragma once

#include <string>
#include <cstdint>

// ----------------------------------------------------------------------------------------- //

/// Valid klotski RawCode sample.
constexpr uint64_t TEST_R_CODE = 0x0603'EDF5'CAFF'F5E2;

/// Valid klotski ShortCode sample.
constexpr uint32_t TEST_S_CODE = 4091296;
constexpr std::string_view TEST_S_CODE_STR = "4WVE1";

/// Valid klotski CommonCode sample.
constexpr uint64_t TEST_C_CODE = 0x1'A9BF'0C00;
constexpr std::string_view TEST_C_CODE_STR = "1A9BF0C00";

// ----------------------------------------------------------------------------------------- //

/// Invalid klotski RawCode sample.
constexpr uint64_t TEST_R_CODE_ERR = 0x1603'ED00'CAFF'F5E2;

/// Invalid klotski ShortCode sample.
constexpr uint32_t TEST_S_CODE_ERR = 1234564323;
constexpr std::string_view TEST_S_CODE_STR_ERR = "ZZZZZZ";

/// Invalid klotski CommonCode sample.
constexpr uint64_t TEST_C_CODE_ERR = 0x3'A9BF'0C00;
constexpr std::string_view TEST_C_CODE_STR_ERR = "0123456789";

// ----------------------------------------------------------------------------------------- //

/// CommonCode with horizontal symmetry.
constexpr uint64_t TEST_MIRROR_1 = 0x1A9BF0C00;
constexpr uint64_t TEST_MIRROR_1_VM = 0xDC3BE6800; // vertical mirror
constexpr uint64_t TEST_MIRROR_1_HM = 0x1A9BF0C00; // horizontal mirror

/// CommonCode without vertical or horizontal symmetry.
constexpr uint64_t TEST_MIRROR_2 = 0x4FEA13400;
constexpr uint64_t TEST_MIRROR_2_VM = 0x8346AFC00; // vertical mirror
constexpr uint64_t TEST_MIRROR_2_HM = 0x6BFA47000; // horizontal mirror

// ----------------------------------------------------------------------------------------- //
