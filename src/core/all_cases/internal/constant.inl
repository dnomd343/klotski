#pragma once

#include <array>

namespace klotski::cases {

// ----------------------------------------------------------------------------------------- //

/// Number of cases corresponding to the head value.
constexpr auto ALL_CASES_NUM = std::to_array({
    2942906, 2260392, 2942906, 0,
    2322050, 1876945, 2322050, 0,
    2322050, 1876945, 2322050, 0,
    2942906, 2260392, 2942906, 0,
});

constexpr auto ALL_CASES_NUM_ = array_sum(ALL_CASES_NUM);

constexpr auto ALL_CASES_OFFSET = to_offset(ALL_CASES_NUM);

// ----------------------------------------------------------------------------------------- //

/// Number of ranges corresponding to the type_id.
constexpr auto BASIC_RANGES_NUM = std::to_array({
    1     , 16    , 120   , 560   , 1820 , 4368 , 8008  , 11440 ,
    12870 , 11440 , 8008  , 4368  , 1820 , 560  , 120   , 15    ,
    210   , 1365  , 5460  , 15015 , 30030, 45045, 51480 , 45045 ,
    30030 , 15015 , 5460  , 1365  , 15   , 210  , 1365  , 5460  ,
    15015 , 30030 , 45045 , 51480 , 45045, 30030, 15015 , 5460  ,
    1365  , 91    , 1092  , 6006  , 20020, 45045, 72072 , 84084 ,
    72072 , 45045 , 20020 , 6006  , 182  , 2184 , 12012 , 40040 ,
    90090 , 144144, 168168, 144144, 90090, 40040, 12012 , 91    ,
    1092  , 6006  , 20020 , 45045 , 72072, 84084, 72072 , 45045 ,
    20020 , 6006  , 286   , 2860  , 12870, 34320, 60060 , 72072 ,
    60060 , 34320 , 12870 , 858   , 8580 , 38610, 102960, 180180,
    216216, 180180, 102960, 38610 , 858  , 8580 , 38610 , 102960,
    180180, 216216, 180180, 102960, 38610, 286  , 2860  , 12870 ,
    34320 , 60060 , 72072 , 60060 , 34320, 12870, 495   , 3960  ,
    13860 , 27720 , 34650 , 27720 , 13860, 1980 , 15840 , 55440 ,
    110880, 138600, 110880, 55440 , 2970 , 23760, 83160 , 166320,
    207900, 166320, 83160 , 1980  , 15840, 55440, 110880, 138600,
    110880, 55440 , 495   , 3960  , 13860, 27720, 34650 , 27720 ,
    13860 , 462   , 2772  , 6930  , 9240 , 6930 , 2310  , 13860 ,
    34650 , 46200 , 34650 , 4620  , 27720, 69300, 92400 , 69300 ,
    4620  , 27720 , 69300 , 92400 , 69300, 2310 , 13860 , 34650 ,
    46200 , 34650 , 462   , 2772  , 6930 , 9240 , 6930  , 210   ,
    840   , 1260  , 1260  , 5040  , 7560 , 3150 , 12600 , 18900 ,
    4200  , 16800 , 25200 , 3150  , 12600, 18900, 1260  , 5040  ,
    7560  , 210   , 840   , 1260  , 36   , 252  , 756   , 1260  ,
    1260  , 756   , 252   ,
});

constexpr auto BASIC_RANGES_NUM_ = array_sum(BASIC_RANGES_NUM);

constexpr auto BASIC_RANGES_OFFSET = to_offset(BASIC_RANGES_NUM);

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::cases
