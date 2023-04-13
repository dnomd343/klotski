#pragma once

namespace klotski {

const uint16_t TYPE_ID_INDEX[203] = {
       0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,  256,
     257,  258,  259,  260,  261,  262,  263,  264,  265,  266,  267,  268,  272,  273,  274,  275,
     276,  277,  278,  279,  280,  281,  282,  283,  284,  512,  513,  514,  515,  516,  517,  518,
     519,  520,  521,  522,  528,  529,  530,  531,  532,  533,  534,  535,  536,  537,  538,  544,
     545,  546,  547,  548,  549,  550,  551,  552,  553,  554,  768,  769,  770,  771,  772,  773,
     774,  775,  776,  784,  785,  786,  787,  788,  789,  790,  791,  792,  800,  801,  802,  803,
     804,  805,  806,  807,  808,  816,  817,  818,  819,  820,  821,  822,  823,  824, 1024, 1025,
    1026, 1027, 1028, 1029, 1030, 1040, 1041, 1042, 1043, 1044, 1045, 1046, 1056, 1057, 1058, 1059,
    1060, 1061, 1062, 1072, 1073, 1074, 1075, 1076, 1077, 1078, 1088, 1089, 1090, 1091, 1092, 1093,
    1094, 1280, 1281, 1282, 1283, 1284, 1296, 1297, 1298, 1299, 1300, 1312, 1313, 1314, 1315, 1316,
    1328, 1329, 1330, 1331, 1332, 1344, 1345, 1346, 1347, 1348, 1360, 1361, 1362, 1363, 1364, 1536,
    1537, 1538, 1552, 1553, 1554, 1568, 1569, 1570, 1584, 1585, 1586, 1600, 1601, 1602, 1616, 1617,
    1618, 1632, 1633, 1634, 1792, 1808, 1824, 1840, 1856, 1872, 1888,
};

const uint32_t TYPE_ID_SIZE[203] = {
        12,    192,   1440,   6720,  21840,  52416,  96096, 137280,
    154440, 137280,  96096,  52416,  21840,   6720,   1440,    124,
      1736,  11284,  45136, 124124, 248248, 372372, 425568, 372372,
    248248, 124124,  45136,  11284,    132,   1848,  12012,  48048,
    132132, 264264, 396396, 453024, 396396, 264264, 132132,  48048,
     12012,    512,   6144,  33792, 112640, 253440, 405504, 473088,
    405504, 253440, 112640,  33792,   1044,  12528,  68904, 229680,
    516780, 826848, 964656, 826848, 516780, 229680,  68904,    582,
      6984,  38412, 128040, 288090, 460944, 537768, 460944, 288090,
    128040,  38412,   1092,  10920,  49140, 131040, 229320, 275184,
    229320, 131040,  49140,   3180,  31800, 143100, 381600, 667800,
    801360, 667800, 381600, 143100,   3382,  33820, 152190, 405840,
    710220, 852264, 710220, 405840, 152190,   1320,  13200,  59400,
    158400, 277200, 332640, 277200, 158400,  59400,   1320,  10560,
     36960,  73920,  92400,  73920,  36960,   4680,  37440, 131040,
    262080, 327600, 262080, 131040,   7248,  57984, 202944, 405888,
    507360, 405888, 202944,   5344,  42752, 149632, 299264, 374080,
    299264, 149632,   1632,  13056,  45696,  91392, 114240,  91392,
     45696,    948,   5688,  14220,  18960,  14220,   3444,  20664,
     51660,  68880,  51660,   7120,  42720, 106800, 142400, 106800,
      7284,  43704, 109260, 145680, 109260,   4392,  26352,  65880,
     87840,  65880,   1044,   6264,  15660,  20880,  15660,    396,
      1584,   2376,   1260,   5040,   7560,   3084,  12336,  18504,
      4288,  17152,  25728,   3196,  12784,  19176,   1836,   7344,
     11016,    270,   1080,   1620,     88,    180,    626,    772,
      1036,    464,    342,
};

const uint32_t TYPE_ID_MAX_GROUP_SIZE[203] = {
        12,    192,   1440,   6720,  21840,  52416,  96096, 137280, 154440, 137280,  96096,  52416,
     21840,   6720,   1440,    124,   1736,  11284,  45136, 124124, 248248, 372372, 425568, 372372,
    248248, 124124,  45136,  10836,    132,   1848,  12012,  48048, 132132, 264264, 396396, 453024,
    396396, 264264, 132132,  48048,  11916,    512,   6144,  33792, 112640, 253440, 405504, 473088,
    405504, 253440, 112576,  30104,   1044,  12528,  68904, 229680, 516780, 826848, 964656, 826848,
    516780, 228848,  65848,    582,   6984,  38412, 128040, 288090, 460944, 537768, 460944, 288090,
    128040,  37740,   1092,  10920,  49140, 131040, 229320, 275184, 229320, 130032,  38672,   3180,
     31800, 143100, 381600, 667800, 801360, 667544, 377408, 127676,   3382,  33820, 152190, 405840,
    710220, 852264, 709388, 403056, 143462,   1320,  13200,  59400, 158400, 277200, 332640, 277200,
    158272,  57524,   1320,  10560,  36960,  73920,  92400,  71944,  23704,   4680,  37440, 131040,
    262080, 327120, 253968, 105064,   7248,  57984, 202944, 405440, 504616, 396284, 175580,   5344,
     42752, 149632, 299136, 373184, 296520, 136040,   1632,  13056,  45696,  91392, 114240,  91264,
     43704,    948,   5688,  14220,  17696,   7888,   3444,  20664,  51044,  61204,  28832,   7048,
     42288, 104540, 132556,  81462,   7284,  42728, 104960, 135192,  81340,   4392,  26352,  65688,
     86968,  25955,    522,   3132,   7830,  10440,   7462,    396,    792,    508,    240,    768,
       742,   1804,   2348,   1398,   1504,   5372,   1192,   2984,   4376,    453,    350,   1380,
      1532,     45,     66,     38,     44,      5,     82,     21,     23,     24,     21,
};

// TODO: TYPE_ID_GROUP_NUM

} // namespace klotski
