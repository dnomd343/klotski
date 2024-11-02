#pragma once

#include <array>

#include "group/group.h"

// TODO: move to `.cc` file
using klotski::cases::Ranges;
using klotski::cases::AllCases;
using klotski::codec::CommonCode;
using klotski::cases::BasicRanges;
using klotski::cases::RangesUnion;

using klotski::group::BLOCK_NUM;
using klotski::cases::ALL_CASES_NUM;
using klotski::cases::ALL_CASES_NUM_;
using klotski::cases::BASIC_RANGES_NUM;
using klotski::cases::BASIC_RANGES_NUM_;

// TODO: move to global helper

/// All valid klotski heads.
constexpr auto Heads = std::to_array({
    0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14
});

// TODO: add NonHeads -> {3, 7, 11, 15}
