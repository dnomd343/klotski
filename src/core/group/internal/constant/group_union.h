#pragma once

#include <array>

namespace klotski::cases {

/// The number of groups contained in GroupUnion.
constexpr auto GROUP_NUM = std::to_array<uint16_t>({
    1  , 1  , 1   , 1  , 1   , 1  , 1  , 1   , 1  , 1   , 1   , 1   , 1  , 1   , 1  , 1   ,
    1  , 1  , 1   , 1  , 1   , 1  , 1  , 1   , 1  , 1   , 1   , 13  , 1  , 1   , 1  , 1   ,
    1  , 1  , 1   , 1  , 1   , 1  , 1  , 1   , 5  , 1   , 1   , 1   , 1  , 1   , 1  , 1   ,
    1  , 1  , 3   , 79 , 1   , 1  , 1  , 1   , 1  , 1   , 1   , 1   , 1  , 21  , 141, 1   ,
    1  , 1  , 1   , 1  , 1   , 1  , 1  , 1   , 1  , 17  , 1   , 1   , 1  , 1   , 1  , 1   ,
    1  , 17 , 273 , 1  , 1   , 1  , 1  , 1   , 1  , 9   , 145 , 841 , 1  , 1   , 1  , 1   ,
    1  , 1  , 15  , 97 , 517 , 1  , 1  , 1   , 1  , 1   , 1   , 1   , 5  , 91  , 1  , 1   ,
    1  , 1  , 1   , 59 , 587 , 1  , 1  , 1   , 1  , 21  , 353 , 1741, 1  , 1   , 1  , 7   ,
    81 , 479, 2327, 1  , 1   , 1  , 5  , 33  , 165, 1053, 1   , 1   , 1  , 1   , 1  , 9   ,
    157, 1  , 1   , 1  , 57  , 505, 1  , 1   , 13 , 325 , 1729, 3   , 9  , 99  , 631, 2609,
    1  , 17 , 91  , 487, 2653, 1  , 1  , 11  , 75 , 898 , 2   , 2   , 2  , 2   , 80 , 1   ,
    2  , 138, 14  , 126, 590 , 22 , 183, 1074, 33 , 209 , 1178, 13  , 171, 1185, 6  , 16  ,
    214, 6  , 18  , 54 , 2   , 44 , 40 , 124 , 84 , 70  , 18  ,
});

/// The maximum Group size in each GroupUnion.
constexpr auto MAX_GROUP_SIZE = std::to_array<uint32_t>({
    12    , 192   , 1440  , 6720  , 21840 , 52416 , 96096 , 137280, 154440, 137280, 96096 , 52416 ,
    21840 , 6720  , 1440  , 124   , 1736  , 11284 , 45136 , 124124, 248248, 372372, 425568, 372372,
    248248, 124124, 45136 , 10836 , 132   , 1848  , 12012 , 48048 , 132132, 264264, 396396, 453024,
    396396, 264264, 132132, 48048 , 11916 , 512   , 6144  , 33792 , 112640, 253440, 405504, 473088,
    405504, 253440, 112576, 30104 , 1044  , 12528 , 68904 , 229680, 516780, 826848, 964656, 826848,
    516780, 228848, 65848 , 582   , 6984  , 38412 , 128040, 288090, 460944, 537768, 460944, 288090,
    128040, 37740 , 1092  , 10920 , 49140 , 131040, 229320, 275184, 229320, 130032, 38672 , 3180  ,
    31800 , 143100, 381600, 667800, 801360, 667544, 377408, 127676, 3382  , 33820 , 152190, 405840,
    710220, 852264, 709388, 403056, 143462, 1320  , 13200 , 59400 , 158400, 277200, 332640, 277200,
    158272, 57524 , 1320  , 10560 , 36960 , 73920 , 92400 , 71944 , 23704 , 4680  , 37440 , 131040,
    262080, 327120, 253968, 105064, 7248  , 57984 , 202944, 405440, 504616, 396284, 175580, 5344  ,
    42752 , 149632, 299136, 373184, 296520, 136040, 1632  , 13056 , 45696 , 91392 , 114240, 91264 ,
    43704 , 948   , 5688  , 14220 , 17696 , 7888  , 3444  , 20664 , 51044 , 61204 , 28832 , 7048  ,
    42288 , 104540, 132556, 81462 , 7284  , 42728 , 104960, 135192, 81340 , 4392  , 26352 , 65688 ,
    86968 , 25955 , 522   , 3132  , 7830  , 10440 , 7462  , 396   , 792   , 508   , 240   , 768   ,
    742   , 1804  , 2348  , 1398  , 1504  , 5372  , 1192  , 2984  , 4376  , 453   , 350   , 1380  ,
    1532  , 45    , 66    , 38    , 44    , 5     , 82    , 21    , 23    , 24    , 21    ,
});

/// The number of cases contained in GroupUnion.
constexpr auto GROUP_UNION_SIZE = std::to_array<uint32_t>({
    12    , 192   , 1440  , 6720  , 21840 , 52416 , 96096 , 137280, 154440, 137280, 96096 , 52416 ,
    21840 , 6720  , 1440  , 124   , 1736  , 11284 , 45136 , 124124, 248248, 372372, 425568, 372372,
    248248, 124124, 45136 , 11284 , 132   , 1848  , 12012 , 48048 , 132132, 264264, 396396, 453024,
    396396, 264264, 132132, 48048 , 12012 , 512   , 6144  , 33792 , 112640, 253440, 405504, 473088,
    405504, 253440, 112640, 33792 , 1044  , 12528 , 68904 , 229680, 516780, 826848, 964656, 826848,
    516780, 229680, 68904 , 582   , 6984  , 38412 , 128040, 288090, 460944, 537768, 460944, 288090,
    128040, 38412 , 1092  , 10920 , 49140 , 131040, 229320, 275184, 229320, 131040, 49140 , 3180  ,
    31800 , 143100, 381600, 667800, 801360, 667800, 381600, 143100, 3382  , 33820 , 152190, 405840,
    710220, 852264, 710220, 405840, 152190, 1320  , 13200 , 59400 , 158400, 277200, 332640, 277200,
    158400, 59400 , 1320  , 10560 , 36960 , 73920 , 92400 , 73920 , 36960 , 4680  , 37440 , 131040,
    262080, 327600, 262080, 131040, 7248  , 57984 , 202944, 405888, 507360, 405888, 202944, 5344  ,
    42752 , 149632, 299264, 374080, 299264, 149632, 1632  , 13056 , 45696 , 91392 , 114240, 91392 ,
    45696 , 948   , 5688  , 14220 , 18960 , 14220 , 3444  , 20664 , 51660 , 68880 , 51660 , 7120  ,
    42720 , 106800, 142400, 106800, 7284  , 43704 , 109260, 145680, 109260, 4392  , 26352 , 65880 ,
    87840 , 65880 , 1044  , 6264  , 15660 , 20880 , 15660 , 396   , 1584  , 2376  , 1260  , 5040  ,
    7560  , 3084  , 12336 , 18504 , 4288  , 17152 , 25728 , 3196  , 12784 , 19176 , 1836  , 7344  ,
    11016 , 270   , 1080  , 1620  , 88    , 180   , 626   , 772   , 1036  , 464   , 342   ,
});

/// The number of (1x2 + 2x1) / 2x1 / 1x1 blocks corresponding to different type_id.
constexpr auto BLOCK_NUM = std::to_array<std::tuple<uint8_t, uint8_t, uint8_t>>({
    {0, 0, 0 }, {0, 0, 1 }, {0, 0, 2 }, {0, 0, 3 }, {0, 0, 4 }, {0, 0, 5 }, {0, 0, 6 }, {0, 0, 7 },
    {0, 0, 8 }, {0, 0, 9 }, {0, 0, 10}, {0, 0, 11}, {0, 0, 12}, {0, 0, 13}, {0, 0, 14}, {1, 0, 0 },
    {1, 0, 1 }, {1, 0, 2 }, {1, 0, 3 }, {1, 0, 4 }, {1, 0, 5 }, {1, 0, 6 }, {1, 0, 7 }, {1, 1, 0 },
    {1, 0, 8 }, {1, 1, 8 }, {1, 0, 9 }, {1, 1, 1 }, {1, 1, 9 }, {1, 0, 10}, {1, 1, 10}, {1, 1, 2 },
    {1, 1, 11}, {1, 1, 3 }, {1, 0, 11}, {1, 1, 12}, {1, 0, 12}, {1, 1, 4 }, {1, 1, 5 }, {1, 1, 6 },
    {1, 1, 7 }, {2, 0, 0 }, {2, 0, 1 }, {2, 0, 2 }, {2, 0, 3 }, {2, 0, 4 }, {2, 0, 5 }, {2, 0, 6 },
    {2, 0, 7 }, {2, 0, 8 }, {2, 1, 8 }, {2, 1, 0 }, {2, 0, 9 }, {2, 1, 1 }, {2, 1, 9 }, {2, 1, 2 },
    {2, 1, 10}, {2, 0, 10}, {2, 1, 3 }, {2, 1, 4 }, {2, 1, 5 }, {2, 1, 6 }, {2, 1, 7 }, {2, 2, 0 },
    {2, 2, 1 }, {2, 2, 2 }, {2, 2, 3 }, {2, 2, 4 }, {2, 2, 5 }, {2, 2, 6 }, {2, 2, 7 }, {2, 2, 8 },
    {2, 2, 9 }, {2, 2, 10}, {3, 0, 0 }, {3, 0, 1 }, {3, 0, 2 }, {3, 0, 3 }, {3, 0, 4 }, {3, 0, 5 },
    {3, 0, 6 }, {3, 0, 7 }, {3, 1, 8 }, {3, 0, 8 }, {3, 1, 0 }, {3, 1, 1 }, {3, 1, 2 }, {3, 1, 3 },
    {3, 1, 4 }, {3, 1, 5 }, {3, 1, 6 }, {3, 1, 7 }, {3, 2, 0 }, {3, 2, 1 }, {3, 2, 2 }, {3, 2, 3 },
    {3, 2, 4 }, {3, 2, 5 }, {3, 2, 6 }, {3, 2, 7 }, {3, 3, 0 }, {3, 3, 8 }, {3, 2, 8 }, {3, 3, 1 },
    {3, 3, 2 }, {3, 3, 3 }, {3, 3, 4 }, {3, 3, 5 }, {3, 3, 6 }, {3, 3, 7 }, {4, 0, 0 }, {4, 0, 1 },
    {4, 0, 2 }, {4, 0, 3 }, {4, 0, 4 }, {4, 0, 5 }, {4, 0, 6 }, {4, 1, 0 }, {4, 1, 1 }, {4, 1, 2 },
    {4, 1, 3 }, {4, 1, 4 }, {4, 1, 5 }, {4, 1, 6 }, {4, 2, 0 }, {4, 2, 1 }, {4, 2, 2 }, {4, 2, 3 },
    {4, 2, 4 }, {4, 2, 5 }, {4, 2, 6 }, {4, 3, 0 }, {4, 3, 1 }, {4, 3, 2 }, {4, 3, 3 }, {4, 3, 4 },
    {4, 3, 5 }, {4, 3, 6 }, {4, 4, 0 }, {4, 4, 1 }, {4, 4, 2 }, {4, 4, 3 }, {4, 4, 4 }, {4, 4, 5 },
    {4, 4, 6 }, {5, 0, 0 }, {5, 0, 1 }, {5, 0, 2 }, {5, 0, 3 }, {5, 0, 4 }, {5, 1, 0 }, {5, 1, 1 },
    {5, 1, 2 }, {5, 1, 3 }, {5, 1, 4 }, {5, 2, 0 }, {5, 2, 1 }, {5, 2, 2 }, {5, 2, 3 }, {5, 2, 4 },
    {5, 3, 0 }, {5, 3, 1 }, {5, 3, 2 }, {5, 3, 3 }, {5, 3, 4 }, {5, 4, 0 }, {5, 4, 1 }, {5, 4, 2 },
    {5, 4, 3 }, {5, 4, 4 }, {5, 5, 0 }, {5, 5, 1 }, {5, 5, 2 }, {5, 5, 3 }, {5, 5, 4 }, {6, 0, 0 },
    {6, 0, 1 }, {6, 0, 2 }, {6, 1, 0 }, {6, 1, 1 }, {6, 1, 2 }, {6, 2, 0 }, {6, 2, 1 }, {6, 2, 2 },
    {6, 3, 0 }, {6, 3, 1 }, {6, 3, 2 }, {6, 4, 0 }, {6, 4, 1 }, {6, 4, 2 }, {6, 5, 0 }, {6, 5, 1 },
    {6, 5, 2 }, {6, 6, 0 }, {6, 6, 1 }, {6, 6, 2 }, {7, 0, 0 }, {7, 1, 0 }, {7, 2, 0 }, {7, 3, 0 },
    {7, 4, 0 }, {7, 5, 0 }, {7, 6, 0 },
});

/// The number of cases with different heads in GroupUnion.
constexpr auto GROUP_UNION_CASES_NUM = std::to_array<std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>>({
    {1    , 1    , 1    , 1    }, {16   , 16   , 16   , 16   }, {120  , 120  , 120  , 120  },
    {560  , 560  , 560  , 560  }, {1820 , 1820 , 1820 , 1820 }, {4368 , 4368 , 4368 , 4368 },
    {8008 , 8008 , 8008 , 8008 }, {11440, 11440, 11440, 11440}, {12870, 12870, 12870, 12870},
    {11440, 11440, 11440, 11440}, {8008 , 8008 , 8008 , 8008 }, {4368 , 4368 , 4368 , 4368 },
    {1820 , 1820 , 1820 , 1820 }, {560  , 560  , 560  , 560  }, {120  , 120  , 120  , 120  },
    {11   , 9    , 11   , 9    }, {154  , 126  , 154  , 126  }, {1001 , 819  , 1001 , 819  },
    {4004 , 3276 , 4004 , 3276 }, {11011, 9009 , 11011, 9009 }, {22022, 18018, 22022, 18018},
    {33033, 27027, 33033, 27027}, {37752, 30888, 37752, 30888}, {12   , 12   , 10   , 10   },
    {33033, 27027, 33033, 27027}, {36036, 36036, 30030, 30030}, {22022, 18018, 22022, 18018},
    {168  , 168  , 140  , 140  }, {24024, 24024, 20020, 20020}, {11011, 9009 , 11011, 9009 },
    {12012, 12012, 10010, 10010}, {1092 , 1092 , 910  , 910  }, {4368 , 4368 , 3640 , 3640 },
    {4368 , 4368 , 3640 , 3640 }, {4004 , 3276 , 4004 , 3276 }, {1092 , 1092 , 910  , 910  },
    {1001 , 819  , 1001 , 819  }, {12012, 12012, 10010, 10010}, {24024, 24024, 20020, 20020},
    {36036, 36036, 30030, 30030}, {41184, 41184, 34320, 34320}, {49   , 30   , 49   , 30   },
    {588  , 360  , 588  , 360  }, {3234 , 1980 , 3234 , 1980 }, {10780, 6600 , 10780, 6600 },
    {24255, 14850, 24255, 14850}, {38808, 23760, 38808, 23760}, {45276, 27720, 45276, 27720},
    {38808, 23760, 38808, 23760}, {24255, 14850, 24255, 14850}, {49005, 40590, 41580, 36630},
    {99   , 82   , 84   , 74   }, {10780, 6600 , 10780, 6600 }, {1188 , 984  , 1008 , 888  },
    {21780, 18040, 18480, 16280}, {6534 , 5412 , 5544 , 4884 }, {6534 , 5412 , 5544 , 4884 },
    {3234 , 1980 , 3234 , 1980 }, {21780, 18040, 18480, 16280}, {49005, 40590, 41580, 36630},
    {78408, 64944, 66528, 58608}, {91476, 75768, 77616, 68376}, {78408, 64944, 66528, 58608},
    {58   , 58   , 39   , 39   }, {696  , 696  , 468  , 468  }, {3828 , 3828 , 2574 , 2574 },
    {12760, 12760, 8580 , 8580 }, {28710, 28710, 19305, 19305}, {45936, 45936, 30888, 30888},
    {53592, 53592, 36036, 36036}, {45936, 45936, 30888, 30888}, {28710, 28710, 19305, 19305},
    {12760, 12760, 8580 , 8580 }, {3828 , 3828 , 2574 , 2574 }, {114  , 45   , 114  , 45   },
    {1140 , 450  , 1140 , 450  }, {5130 , 2025 , 5130 , 2025 }, {13680, 5400 , 13680, 5400 },
    {23940, 9450 , 23940, 9450 }, {28728, 11340, 28728, 11340}, {23940, 9450 , 23940, 9450 },
    {13680, 5400 , 13680, 5400 }, {14355, 9270 , 12240, 9090 }, {5130 , 2025 , 5130 , 2025 },
    {319  , 206  , 272  , 202  }, {3190 , 2060 , 2720 , 2020 }, {14355, 9270 , 12240, 9090 },
    {38280, 24720, 32640, 24240}, {66990, 43260, 57120, 42420}, {80388, 51912, 68544, 50904},
    {66990, 43260, 57120, 42420}, {38280, 24720, 32640, 24240}, {346  , 288  , 243  , 225  },
    {3460 , 2880 , 2430 , 2250 }, {15570, 12960, 10935, 10125}, {41520, 34560, 29160, 27000},
    {72660, 60480, 51030, 47250}, {87192, 72576, 61236, 56700}, {72660, 60480, 51030, 47250},
    {41520, 34560, 29160, 27000}, {144  , 144  , 76   , 76   }, {6480 , 6480 , 3420 , 3420 },
    {15570, 12960, 10935, 10125}, {1440 , 1440 , 760  , 760  }, {6480 , 6480 , 3420 , 3420 },
    {17280, 17280, 9120 , 9120 }, {30240, 30240, 15960, 15960}, {36288, 36288, 19152, 19152},
    {30240, 30240, 15960, 15960}, {17280, 17280, 9120 , 9120 }, {150  , 30   , 150  , 30   },
    {1200 , 240  , 1200 , 240  }, {4200 , 840  , 4200 , 840  }, {8400 , 1680 , 8400 , 1680 },
    {10500, 2100 , 10500, 2100 }, {8400 , 1680 , 8400 , 1680 }, {4200 , 840  , 4200 , 840  },
    {507  , 230  , 426  , 244  }, {4056 , 1840 , 3408 , 1952 }, {14196, 6440 , 11928, 6832 },
    {28392, 12880, 23856, 13664}, {35490, 16100, 29820, 17080}, {28392, 12880, 23856, 13664},
    {14196, 6440 , 11928, 6832 }, {767  , 509  , 557  , 467  }, {6136 , 4072 , 4456 , 3736 },
    {21476, 14252, 15596, 13076}, {42952, 28504, 31192, 26152}, {53690, 35630, 38990, 32690},
    {42952, 28504, 31192, 26152}, {21476, 14252, 15596, 13076}, {593  , 498  , 334  , 320  },
    {4744 , 3984 , 2672 , 2560 }, {16604, 13944, 9352 , 8960 }, {33208, 27888, 18704, 17920},
    {41510, 34860, 23380, 22400}, {33208, 27888, 18704, 17920}, {16604, 13944, 9352 , 8960 },
    {193  , 193  , 79   , 79   }, {1544 , 1544 , 632  , 632  }, {5404 , 5404 , 2212 , 2212 },
    {10808, 10808, 4424 , 4424 }, {13510, 13510, 5530 , 5530 }, {10808, 10808, 4424 , 4424 },
    {5404 , 5404 , 2212 , 2212 }, {114  , 9    , 114  , 9    }, {684  , 54   , 684  , 54   },
    {1710 , 135  , 1710 , 135  }, {2280 , 180  , 2280 , 180  }, {1710 , 135  , 1710 , 135  },
    {413  , 106  , 334  , 122  }, {2478 , 636  , 2004 , 732  }, {6195 , 1590 , 5010 , 1830 },
    {8260 , 2120 , 6680 , 2440 }, {6195 , 1590 , 5010 , 1830 }, {788  , 402  , 579  , 424  },
    {4728 , 2412 , 3474 , 2544 }, {11820, 6030 , 8685 , 6360 }, {15760, 8040 , 11580, 8480 },
    {11820, 6030 , 8685 , 6360 }, {829  , 550  , 498  , 438  }, {4974 , 3300 , 2988 , 2628 },
    {12435, 8250 , 7470 , 6570 }, {16580, 11000, 9960 , 8760 }, {12435, 8250 , 7470 , 6570 },
    {521  , 453  , 235  , 231  }, {3126 , 2718 , 1410 , 1386 }, {7815 , 6795 , 3525 , 3465 },
    {10420, 9060 , 4700 , 4620 }, {7815 , 6795 , 3525 , 3465 }, {132  , 132  , 42   , 42   },
    {792  , 792  , 252  , 252  }, {1980 , 1980 , 630  , 630  }, {2640 , 2640 , 840  , 840  },
    {1980 , 1980 , 630  , 630  }, {49   , 1    , 49   , 1    }, {196  , 4    , 196  , 4    },
    {294  , 6    , 294  , 6    }, {165  , 22   , 126  , 26   }, {660  , 88   , 504  , 104  },
    {990  , 132  , 756  , 156  }, {374  , 115  , 269  , 141  }, {1496 , 460  , 1076 , 564  },
    {2244 , 690  , 1614 , 846  }, {491  , 282  , 310  , 260  }, {1964 , 1128 , 1240 , 1040 },
    {2946 , 1692 , 1860 , 1560 }, {394  , 254  , 197  , 162  }, {1576 , 1016 , 788  , 648  },
    {2364 , 1524 , 1182 , 972  }, {224  , 212  , 86   , 86   }, {896  , 848  , 344  , 344  },
    {1344 , 1272 , 516  , 516  }, {36   , 36   , 9    , 9    }, {144  , 144  , 36   , 36   },
    {216  , 216  , 54   , 54   }, {11   , 0    , 11   , 0    }, {25   , 2    , 18   , 2    },
    {83   , 14   , 57   , 19   }, {94   , 38   , 61   , 38   }, {126  , 73   , 69   , 55   },
    {60   , 40   , 28   , 16   }, {42   , 42   , 15   , 15   },
});

} // namespace klotski::cases
